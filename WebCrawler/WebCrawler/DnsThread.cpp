#include "DnsThread.h"



void * DnsThread::run(void)
{
	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "DNS�߳̿�ʼ");

	for (;;)
	{
		DnsUrl dnsUrl = static_cast<DnsUrl>(g_app->m_urlQueue.popRawUrl());

		map<string, string>::const_iterator it = s_hosts.find(dnsUrl.m_domain);

		if (it != s_hosts.end())
		{
			dnsUrl.m_ip = it->second;
			g_app->m_urlQueue.pushDnsUrl(dnsUrl);
			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "����\"%s\"����������Ϊ\"%s\"", dnsUrl.m_domain.c_str(), dnsUrl.m_ip.c_str());

			continue;
		}

		hostent *host = gethostbyname(dnsUrl.m_domain.c_str());

		if (!host)
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__,
				"gethostbyname: %s", hstreror(h_errno));

			continue;
		}

		if (host->h_addrtype != AF_INET)
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "��Ч��ַ����");

			continue;
		}

		s_hosts[dnsUrl.m_domain] = dnsUrl.m_ip = inet_ntoa(**(int_addr**)host->h_addr_list);

		g_app->m_urlQueue.pushDnsUrl(dnsUrl);

		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__,
			"����\%s\"���ɹ�����Ϊ\"%s\"", dnsUrl.m_domain.c_str(), dnsUrl.m_ip.c_str());
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "DNS�߳���ֹ");

	return NULL;
}

map<string, string> DnsThread::s_hosts;
