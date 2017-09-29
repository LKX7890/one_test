#include "Socket.h"



Socket::Socket(DnsUrl const &dnsUrl) : m_sockfd(-1), m_dnsUrl(dnsUrl)
{

}

~Socket::~Socket(void)
{
	if (m_sockfd >= 0)
	{
		close(m_sockfd);
		m_sockfd = -1;
	}
}

bool Socket::sendRequest(void)
{
	if ((m_sockfd = socket(PF_INET, SOCK_STREA, 0)) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "socket: %s", strerror(errno));

		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "创建套接字%d成功"， m_sockfd);

	sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	add.sin_family = AF_INET;
	addr.sin_port = htons(m_dnsUrl.m_port);

	if (!inet_aton(m_dnsUrl.m_ip.c_str(), &(addr.sin_addr)))
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "inet_aton: %s", strerror(errno));

		return false;
	}

}


