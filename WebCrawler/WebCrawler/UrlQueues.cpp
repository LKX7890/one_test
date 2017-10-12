#include "UrlQueues.h"



UrlQueues::UrlQueues(UrlFilter &filter) : m_filter(filter)
{
	pthread_mutex_init(&m_rawUrlMutex, NULL);
	pthread_mutex_init(&m_rawUrlNoFull, NULL);
	pthread_mutex_init(&m_rawUrlNoEmpty, NULL);
	pthread_mutex_init(&m_dnsUrlMutex, NULL);
	pthread_mutex_init(&m_dnsUrlNoFull, NULL);
	pthread_mutex_init(&m_dnsUrlNoEmpty, NULL);
}

UrlQueues::~UrlQueues()
{
	pthread_cond_destroy(&m_dnsUrlNoEmpty);
	pthread_cond_destroy(&m_dnsUrlNoFull);
	pthread_cond_destroy(&m_dnsUrlMutex);
	pthread_cond_destroy(&m_rawUrlNoEmpty);
	pthread_cond_destroy(&m_rawUrlNoFull);
	pthread_cond_destroy(&m_rawUrlMutex);
}

void UrlQueues::pushRawUrl(RawUrl const &rawUrl)
{
	pthread_mutex_lock(&m_rawUrlMutex);

	// 判断是否已经处理过
	if (m_filter.exist(rawUrl.m_strUrl))
	{
		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "不再处理已处理过的统一资源定位符\"%s\"",
			rawUrl.m_strUrl.c_str());
	}

	else if (g_app->m_pluginMngr.RegisterUrlPlugin(const_cast<RawUrl*>(&rawUrl)))
	{
		// 达到上限，挂起
		while (0 <= g_app->m_cfg.maxRawUrls && (size_t)g_app->m_cfg.m_maxRawUrls <= m_rawUrlQueue.size())
		{
			pthread_cond_wait(&m_rawUrlNoFull, &m_rawUrlMutex);
		}

		// 没达到上限
		m_rawUrlQueue.push_back(rawUrl);

		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "原始统一资源定位符\"%s\"入队", rawUrl.m_strUrl.c_str());

		// 当有一个时，通知
		if (m_rawUrlQueue.size() == 1)
		{
			pthread_cond_signal(&m_rawUrlNoEmpty);
		}
	}
}

RawUrl UrlQueues::popRawUrl(void)
{
	pthread_mutex_lock(&m_rawUrlMutex);

	while (m_rawUrlQueue.empty())
	{
		pthread_cond_wait(&m_rawUrlNoEmpty, &m_rawUrlMutex);
	}

	RawUrl rawUrl = m_rawUrlQueue.front();

	m_rawUrlQueue.pop_front();

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "原始统一资源定位符\"%s\"出队", rawUrl.m_strUrl.c_str());

	// 当没达到极限时，通知非满
	if (m_rawUrlQueue.size() == (size_t)g_app->m_cfg.m_maxRawUrls - 1)
	{
		pthread_cond_signal(&m_rawUrlNoFull);
	}

	// 解锁
	pthread_mutex_unlock(&m_rawUrlMutex);

	return rawUrl;
}

size_t UrlQueues::sizeRawUrl(void) const
{
	pthread_mutex_lock(&m_rawUrlMutex);

	size_t size = m_rawUrlQueue.size();

	pthread_mutex_unlock(&m_rawUrlMutex);

	return size;
}

bool UrlQueues::emptyRawUrl(void) const
{
	pthread_mutex_lock(&m_rawUrlMutex);

	bool empty = m_rawUrlQueue.empty();

	pthread_mutex_unlock(&m_rawUrlMutex);

	return empty;
}

bool UrlQueues::fullRawUrl(void) const
{
	pthread_mutex_lock(&m_rawUrlMutex);

	bool full = 0 <= g_app->m_cfg.m_maxRawUrls && (size_t)g_app->m_cfg.m_maxRawUrls <= m_rawUrlQueue.size();

	pthread_mutex_unlock(&m_rawUrlMutex);

	return full;
}

void UrlQueues::clearRawUrl(void)
{
	pthread_mutex_lock(&m_rawUrlMutex);

	m_rawUrlQueue.clear();

	pthread_cond_signal(&m_rawUrlNoFull);

	pthread_mutex_unlock(&m_rawUrlMutex);
}

void UrlQueues::pushDnsUrl(DnsUrl const *dnsUrl)
{
	pthread_mutex_lock(&m_dnsUrlMutex);

	while (0 <= g_app->m_cfg.m_maxDnsUrls && (size_t)g_app->m_cfg.m_maxDnsUrls <= m_dnsUrlQueue.size())
	{
		pthread_cond_wait(&m_dnsUrlNoFull, &m_dnsUrlMutex);
	}

	m_dnsUrlQueue.push_back(dnsUrl);

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "解析统一资源定位符\"ip=%s, port=%d, path=%s\"入队",
		dnsUrl.m_ip.c_str(), dnsUrl.m_port, dnsUrl.m_path.c_str());

	if (m_dnsUrlQueue.size() == 1)
	{
		pthread_cond_signal(&m_dnsUrlNoEmpty);
	}

	pthread_mutex_unlock(&m_dnsUrlMutex);
}

DnsUrl UrlQueues::popDnsUrl(void)
{
	pthread_mutex_lock(&m_dnsUrlMutex);

	while (m_dnsUrlQueue.empty())
	{
		pthread_cond_wait(&m_dnsUrlNoEmpty, &m_dnsUrlMutex);
	}

	DnsUrl dnsUrl = m_dnsUrlQueue.front();

	m_dnsUrlQueue.pop_front();

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "解析统一资源定位符\"ip=%s, port=%s, path=%s\"出队",
		dnsUrl.m_ip.c_str(), dnsUrl.m_port, dnsUrl.m_path.c_str());

	if (m_dnsUrlQueue.size(== (size_t)g_app->m_cfg.m_maxDnsUrls - 1))
	{
		pthread_cond_signal(&m_dnsUrlNoFull);
	}

	pthread_mutex_unlock(&m_dnsUrlMutex);

	return dnsUrl;
}

size_t UrlQueues::sizeDnsUrl(void) const
{
	pthread_mutex_lock(&m_dnsUrlMutex);

	size_t size = m_dnsUrlQueue.size();

	pthread_mutex_unlock(&m_dnsUrlMutex);

	return size;
}

bool UrlQueues::emptyDnsUrl(void) const
{
	pthread_mutex_lock(&m_dnsUrlMutex);

	bool empty = m_dnsUrlQueue.empty();

	pthread_mutex_unlock(&m_dnsUrlMutex);

	return empty;
}

bool UrlQueues::fullDnsUrl(void) const
{
	pthread_mutex_lock(&m_dnsUrlMutex);

	bool full = 0 <= g_app->m_cfg.m_maxDnsUrls && (size_t)g_app->m_cfg.m_maxDnsUrls <= m_dnsUrlQueue.size();

	pthread_mutex_unlock(&m_dnsUrlMutex);

	return full;
}

void UrlQueues::clearDnsUrl(void)
{
	pthread_mutex_lock(&m_dnsUrlMutex);

	m_dnsUrlQueue.clear();

	pthread_cond_signal(&m_dnsUrlNoFull);

	pthread_mutex_unlock(&m_dnsUrlMutex);
}

void UrlQueues::extractUrl(char const *html, DnsUrl const &dnsUrl)
{
	regex_t ex;

	int error = regcomp(&ex, "href=\"\\s*\\([^ >\"]*\\)\\s*\"", 0);

	if (error)
	{
		char errInfo[1024];

		regerror(error, &ex, errInfo, sizeof(errInfo) / sizeof(errInfo[0]));

		g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "regcomp:%s", errInfo);
	}

	regmatch_t match[2];

	while (regexec(&ex, html, sizeof(match) / sizeof(match[0], match, 0)) != REG_NOMATCH)
	{
		html += match[1].rm_so;

		size_t len = match[1].rm_eo - match[1].rm_so;

		string strUrl(html, len);

		html += len + match[0].rm_eo - match[1].rm_eo;

		if (isBinary(strUrl))
		{
			continue;
		}

		if (!dnsUrl.attachDomain(strUrl))
		{
			continue;
		}

		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "抽取到一个深度为%d的统一资源定位符\"%s\"",
			dnsUrl.m_depth + 1, strUrl.c_str());

		if (!RawUrl::FormattingRawUrl(strUrl))
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "规格化统一资源定位符\"%s\"", strUrl.c_str());
			continue;
		}

		pushRawUrl(RawUrl(strUrl, RawUrl::ETYPE_HTML, dnsUrl.m_depth + 1)；
	}

	regfree(&ex);
}

bool UrlQueues::isBinary(string const &strUrl)
{
	string::size_type pos;
	return (pos = strUrl.find_last_of('.')) != string::npos && string(".jpg.jpeg.gif.png.ico.bmp.swf").find(strUrl.substr(pos)) != string::npos;
}
