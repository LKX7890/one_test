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

}

void WebCrawler::StartJob(void)
{

}

void WebCrawler::StopJob(bool success /*= true*/)
{

}

void WebCrawler::Sigalrm(int signum)
{

}

void WebCrawler::InitDaemon(void) const
{

}

bool WebCrawler::InitMaxFiles(rlim_t maxFiles) const
{

}

void WebCrawler::InitSeeds(void)
{

}

void WebCrawler::InitDns(void)
{

}

void WebCrawler::InitTicker(void) const
{

}

void WebCrawler::InitSend(void)
{

}
