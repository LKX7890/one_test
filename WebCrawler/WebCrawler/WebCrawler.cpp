#include "WebCrawler.h"



WebCrawler::WebCrawler(UrlFilter &filter) : m_urlQueues(filter), m_cur_jobs(0), m_start(time(NULL)),
m_success(0), m_failure(0)
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond, NULL);
}

WebCrawler::~WebCrawler()
{
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
}

void WebCrawler::Init(bool daemon /*= false*/)
{
	m_cfg.LoadCfgFile("WebCrawler.cfg");

	if (daemon)
	{
		InitDaemon();
	}

	m_pluginMngr.LoadPlugins();

	if (chdir("../download") == -1)
	{
		m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "chdir: %s", strerror(errno));
	}

	InitMaxFiles(1024);
	InitSeeds();
	InitDns();
	InitTicker();
	InitSend();
}

void WebCrawler::ExecMultiIO()
{
	for (;;)
	{
		epoll_event events[10];
		int fds = m_multiIO.wait(events, sizeof(events) / sizeof(events[0]), 2000);
		if (fds <= 0)
		{
			if (!m_cur_jobs && m_urlQueues.emptyRawUrl() && m_urlQueues.emptyDnsUrl())
			{
				Sleep(1);

				if (!m_cur_jobs && m_urlQueues.emptyRawUrl() && m_urlQueues.emptyDnsUrl())
				{
					break;
				}
			}
			else
			{
				if (m_cur_jobs == m_cfg.m_max_jobs && m_urlQueues.fullRawUrl() && m_urlQueues.fullDnsUrl())
				{
					Sleep(1);

					if (m_cur_jobs == m_cfg.m_max_jobs && m_urlQueues.fullRawUrl() && m_urlQueues.fullDnsUrl())
					{
						m_urlQueues.clearRawUrl();
					}
				}
			}
		}

		for (int i = 0; i < fds; ++i)
		{
			Socket *socket = (Socket*)evnets[i].data.ptr;

			if (events[i].events & EPOOLLERR || events[i].events &EPOLLHUP || !(events[i].events & EPOLLIN))
			{
				m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "套接字异常");
				m_multiIO.del(socket->m_sockfd(), events[i]);

				delete socket;
				continue;
			}

			m_multiIO.del(socket->sockfd(), events[i]);
			m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, socket->sockfd());

			(new RecvThread(socket))->ThreadStart();
		}
	}

	m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "任务完成");
}

void WebCrawler::StartJob(void)
{
	pthread_mutex_lock(&m_mutex);

	while (m_cur_jobs == m_cfg.m_max_jobs)
	{
		pthread_cond_wait(&m_cond, &m_mutex);
	}

	DnsUrl dnsUrl = m_urlQueues.popDnsUrl();
	Socket *socket = new Socket(dnsUrl);

	if (socket->sendRequest())
	{
		++m_cur_jobs;
	}
	else
	{
		delete socket;
	}
}

void WebCrawler::StopJob(bool success /*= true*/)
{
	pthread_mutex_lock(&m_mutex);
	if (--m_cur_jobs == m_cfg.m_max_jobs - 1)
	{
		pthread_cond_signal(&m_cond);
	}

	if (success)
	{
		++m_success;
	}
	else
	{
		++m_failure;
	}

	pthread_mutex_unlock(&m_mutex);
}

void WebCrawler::Sigalrm(int signum)
{
	time_t t = time(NULL) - g_app->m_start;

	g_app->m_log.printf(Log::LEVEL_INF, __FILE__, __LINE__,
		"当前任务  原始队列  解析队列  抓取时间  成功次数  失败次数  成功率\n"
		"--------------------------------------------------------\n"
		"%8d %8u %02d:%02d:%02d %8u %8u %5u%%", g_app->m_cur_jobs,
		g_app->m_urlQueue.sizeRawUrl(), g_app->m_urlQueues.sizeDnsUrl(),
		t / 3600, t % 3600 / 60, t % 60, g_app->m_success, g_app->m_failure,
		g_app->m_success ? g_app->m_success * 100 / (g_app->m_success + g_app->m_failure) : 0);
}

void WebCrawler::InitDaemon(void) const
{
	pid_t pid = fork();
	if (pid == -1)
	{
		m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "fork: %s", strerror(errno));
	}

	if (pid)
	{
		exit(EXIT_SUCCESS);
	}

	setsid();

	int fd = open("/dev/null", O_RDWR, 0);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		if (fd > STDERR_FILENO)
		{
			close(fd);
		}
	}

	if (!m_cfg.m_log_file.empty() && (fd = open(m_cfg.m_log_file.c_str(), O_WRONLY | OAPPEND | O_CREAT, 0664)) != -1)
	{
		dup(fd, STDOUT_FILENO);
		if (fd > STDERR_FILENO)
		{
			close;
		}
	}
}

bool WebCrawler::InitMaxFiles(rlim_t maxFiles) const
{
	rlimit rl;

	if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
	{
		m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "getrlimit: %s", strerror(errno));
		return false;
	}

	if (maxFiles > rl.rlim_max)
	{
		m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "文件描述符上限不能超过%lu", rl.rlim_max);
		return false;
	}

	rl.rlim_cur = maxFiles;
	if (setrlimit(RLIMIT_NOFILE, &rl) == -1)
	{
		m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "setrlimit: %s", strerror(errno));
		return false;
	}

	return true;
}

void WebCrawler::InitSeeds(void)
{
	if (m_cfg.m_seeds.empty())
	{
		m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "没种子")；
	}

	vector<string> seeds = StrKit::StrSplit(m_cfg.m_seeds, ",", 0);

	for (vector<string>::iterator it = seeds.begin(); it != seeds.end(); ++it)
	{
		if (RawUrl::normalized(*it))
		{
			m_urlQueues.pushRawUrl(RawUrl(*it));
		}
	}
}

void WebCrawler::InitDns(void)
{
	m_dnsThread.ThreadStart();
}

void WebCrawler::InitTicker(void) const
{
	if (m_cfg.m_stat_interval > 0)
	{
		if (signal(Sigalrm, sigalrm) == SIG_ERR)
		{
			m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "signal: %s", strerror(errno));
		}

		itimerval it;
		it.it_value.tv_sec = m_cfg.m_stat_interval;
		it.it_value.tv_use = 0;
		it.it_interval.tv_sec = m_cfg.m_stat_interval;
		it.it_interval.tv_usec = 0;

		if (setitimer(ITIMER_REAL, &it, NULL) == -1)
		{
			m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "setitimer: %s", strerror(errno));
		}
	}
}

void WebCrawler::InitSend(void)
{
	m_sendThread.ThreadStart();
}
