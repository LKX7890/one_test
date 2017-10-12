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
	// 创建套接字
	if ((m_sockfd = socket(PF_INET, SOCK_STREA, 0)) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "socket: %s", strerror(errno));

		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "创建套接字%d成功"， m_sockfd);

	// 初始化
	sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	add.sin_family = AF_INET;
	addr.sin_port = htons(m_dnsUrl.m_port);

	if (!inet_aton(m_dnsUrl.m_ip.c_str(), &(addr.sin_addr)))
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "inet_aton: %s", strerror(errno));

		return false;
	}

	// 请求连接
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

	// 获取流字符串
	string request = oss.str();
	char const *buf = request.c_str();

	// 循环发送
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

	// 关注此套接字
	epoll_event event = { EPOLLIN | EPOLLET, this };

	// 将此套接字加入多路io
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

		// 接收，就一个字符
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

		// 服务器关闭连接
		if (!rlen)
		{
			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收超文本传输协议响应包%u字节"， res.m_len);

			// 抽出Url
			if (res.m_header.m_contentType.find("text/html", 0) != string::npos)
			{
				g_app->m_urlQueues.extractUrl(res.m_body, m_dnsUrl);
			}

			g_app->m_pluginMngr.UrlPluginHandle(&res);

			break;
		}

		// 按现存包体长度写入
		res.m_body = (char *)realloc(res.m_body, res.m_len + rlen + sizeof(res.m_body[0]));

		memcpy(res.m_body + res.m_len, buf, rlen + sizeof(res.m_body[0]));
		res.m_len += rlen;  // 增加包体长度，不要'\0'

		// 若包头未解析
		if (!headerParsed)
		{
			// 查找包头结束语
			char *p = strstr(res.m_body, "\r\n\r\n");
			if (p)
			{
				string header(res.m_body, p - res.m_body);

				g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收超文本传输协议响应包头%u字节\n\n%s",
					header.size(), header.c_str());

				res.m_header = parseHeader(header);

				if (!g_app->m_pluginMngr.HeaderPluginHandle(&res.m_header)) return;

				headerParsed = true;

				// 跳过结束语位置后的位置减去起点位置
				p += 4;
				res.m_len -= p - res.m_body;

				char *tmp = new char[res.m_len + sizeof(res.m_body[0])];

				// 此处虽然添加了空字符，当再次接收是按现存实际包体长度后写入
				memcpy(tmp, p, res.m_len + sizeof(res.m_body[0]));
				memcpy(res.m_body, tmp, res.m_len + sizeof(res.m_body[0]));

				delete[] tmp;
			}
		}
	}

	return true;
}

int Socket::sockfd(void) const
{
	return m_sockfd;
}

HttpHeader Socket::parseHeader(string str) const
{
	HttpHeader header = {};

	string::size_type pos = str.find("\r\n", 0);
	if (pos != string::npos)
	{
		vector<string> strv = StrKit::StrSplit(str.substr(0, pos), " ", 2);

		if (strv.size() == 3)
		{
			// 获得状态吗
			header.m_statusCode = atoi(strv[1].c_str());
		}
		else
		{
			// 状态码为600，即无法解析的响应包
			header.m_statusCode = 600;
		}

		str = str.substr(pos + 2)；
	}

	while ((pos = str.find("\r\n", 0)) != string::npos)
	{
		// 解析内容类型
		vector<string> strv = StrKit::StrSplit(str.substr(0, pos), ":", 1);
		if (strv.size() == 2 && !strcasecmp(strv[0].c_str(), "content-type"))
		{
			header.m_contentType = strv[1];
			break;
		}

		str = str.substr(pos + 2);
	}

	return header;
}

