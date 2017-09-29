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

	if (connect(m_sockfd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "connect: %s", strerror(errno));

		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "连接服务器\"%s\"成功", m_dnsUrl.m_ip.c_str());

	int flage = fcntl(m_sockfd, F_GETFL);
	if (-1 == flage)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "fcntl: %s", strerror(errno));

		return false;
	}

	ostringstream oss;
	oss <<
		"GET /" << m_dnsUrl.m_path << " HTTP/1.0\r\n"
		"Host: " << m_dnsUrl.m_domain << "\r\n"
		"Accept: */*\r\n"
		"Connection: Keep-Alive\r\n"
		"User-Agent: Mozilla/5.0 (compatible; Qteqpidspider/1.0;)\r\n"
		"Referer: " << m_dnsUrl.m_domain << "\r\n\r\n";

	string request = oss.str();
	char const *buf = request.c_str();

	SSIZE_T slen;

	for (size_t len = request.size(); len; len -= slen, buf += slen)
	{
		if ((slen = send(m_sockfd, buf, len, 0)) == -1)
		{
			if (error == EAGAIN || errno == EWOULDBLOCK)
			{
				usleep(1000);
				slen = 0;
				continue;
			}
		}

		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "send: %s", strerror(errno));

		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "发送超文本传输协议请求包%u字节\n\n%s", request.size(), request.c_str());

	epoll_event event = { EPOLLIN | EPOLLET, this };

	if (!g_app->m_multiIo.add(m_sockfd, event))
	{
		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "关注套接字%d上的IO事件", m_mockfd);

	return true;
}

bool Socket::recvResponse(void)
{
	HttpResponse res(m_dnsUrl);

	bool headerParsed = false;

	for (;;)
	{
		char buf[1024] = {};

		ssize_t rlen = recv(m_sockfd, buf, sizeof(buf) - sizeof(buf[0]), 0);

		if (-1 == rlen)
		{
			if (errno == EAGAIN || ERRNO == EWOULDBLOCK || errno == EINTR)
			{
				usleep(100000);
				continue;
			}

			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "recv: %s", strerror(errno));

			return false;
		}

		if (!rlen)
		{
			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收超文本传输协议响应保胎%u字节"， res.m_len);

			if (res.m_header.m_contentType.find("text/html", 0) != string::npos)
			{
				g_app->m_urlQueues.extractUrl(res.m_body, m_dnsUrl);
			}

			g_app->m_pluginMngr.RegisterHtmlPlugin(&res);

			break;
		}

		res.m_body = (char *)realloc(res.m_body, res.m_len + rlen + sizeof(res.m_body[0]));

		memcpy(res.m_body + res.m_len, buf, rlen + sizeof(res.m_body[0]));
		res.m_len += rlen;

		if (!headerParsed)
		{
			char *p = strstr(res.m_body, "\r\n\r\n");
			if (p)
			{
				string header(res.m_body, p - res.m_body);

				g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收超文本传输协议响应包头%u字节\n\n%s",
					header.size(), header.c_str());

				res.m_header = parseHeader(header);

				if (!g_app->m_pluginMngr.HeaderPluginHandle(&res.m_header)) return;

				headerParsed = true;

				p += 4;
				res.m_len -= p - res.m_body;
				char *tmp = new char[res.m_len + sizeof(res.m_body[0])];
				memcpy(tmp, p, res.m_len + sizeof(res.m_body[0]));
				memcpy(res.m_body, tmp, res.m_len + sizeof(res.m_body[0]));

				delete[] tmp;
			}
		}
	}

	return true;
}

