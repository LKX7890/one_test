#include "DnsThread.h"



void * DnsThread::ThreadHandlerFunc(void)
{
	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "DNS线程开始");

	for (;;)
	{
		DnsUrl dnsUrl = static_cast<DnsUrl>(g_app->m_urlQueue.popRawUrl());

		// map中存在
		map<string, string>::const_iterator it = s_hosts.find(dnsUrl.m_domain);

		if (it != s_hosts.end())
		{
			dnsUrl.m_ip = it->second;
			g_app->m_urlQueue.pushDnsUrl(dnsUrl);
			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "域名\"%s\"曾经被解析为\"%s\"", dnsUrl.m_domain.c_str(), dnsUrl.m_ip.c_str());

			continue;
		}

		// map中不存在时进行解析
		hostent *host = gethostbyname(dnsUrl.m_domain.c_str());

		if (!host)
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__,"gethostbyname: %s", hstreror(h_errno));
			continue;
		}

		if (host->h_addrtype != AF_INET)
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "无效地址类型");
			continue;
		}

		// map ip 赋值
		s_hosts[dnsUrl.m_domain] = dnsUrl.m_ip = inet_ntoa(**(int_addr**)host->h_addr_list);

		g_app->m_urlQueue.pushDnsUrl(dnsUrl);

		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__,
			"域名\%s\"被成功解析为\"%s\"", dnsUrl.m_domain.c_str(), dnsUrl.m_ip.c_str());
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "DNS线程终止");

	return NULL;
}

map<string, string> DnsThread::s_hosts;
