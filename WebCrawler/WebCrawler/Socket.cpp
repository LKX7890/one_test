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
	// �����׽���
	if ((m_sockfd = socket(PF_INET, SOCK_STREA, 0)) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "socket: %s", strerror(errno));

		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�����׽���%d�ɹ�"�� m_sockfd);

	// ��ʼ��
	sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	add.sin_family = AF_INET;
	addr.sin_port = htons(m_dnsUrl.m_port);

	if (!inet_aton(m_dnsUrl.m_ip.c_str(), &(addr.sin_addr)))
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "inet_aton: %s", strerror(errno));

		return false;
	}

	// ��������
	if (connect(m_sockfd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "connect: %s", strerror(errno));

		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "���ӷ�����\"%s\"�ɹ�", m_dnsUrl.m_ip.c_str());

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

	// ��ȡ���ַ���
	string request = oss.str();
	char const *buf = request.c_str();

	// ѭ������
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

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "���ͳ��ı�����Э�������%u�ֽ�\n\n%s", request.size(), request.c_str());

	// ��ע���׽���
	epoll_event event = { EPOLLIN | EPOLLET, this };

	// �����׽��ּ����·io
	if (!g_app->m_multiIo.add(m_sockfd, event))
	{
		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "��ע�׽���%d�ϵ�IO�¼�", m_mockfd);

	return true;
}

bool Socket::recvResponse(void)
{
	HttpResponse res(m_dnsUrl);

	bool headerParsed = false;

	for (;;)
	{
		char buf[1024] = {};

		// ���գ���һ���ַ�
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

		// �������ر�����
		if (!rlen)
		{
			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "���ճ��ı�����Э����Ӧ��%u�ֽ�"�� res.m_len);

			// ���Url
			if (res.m_header.m_contentType.find("text/html", 0) != string::npos)
			{
				g_app->m_urlQueues.extractUrl(res.m_body, m_dnsUrl);
			}

			g_app->m_pluginMngr.UrlPluginHandle(&res);

			break;
		}

		// ���ִ���峤��д��
		res.m_body = (char *)realloc(res.m_body, res.m_len + rlen + sizeof(res.m_body[0]));

		memcpy(res.m_body + res.m_len, buf, rlen + sizeof(res.m_body[0]));
		res.m_len += rlen;  // ���Ӱ��峤�ȣ���Ҫ'\0'

		// ����ͷδ����
		if (!headerParsed)
		{
			// ���Ұ�ͷ������
			char *p = strstr(res.m_body, "\r\n\r\n");
			if (p)
			{
				string header(res.m_body, p - res.m_body);

				g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "���ճ��ı�����Э����Ӧ��ͷ%u�ֽ�\n\n%s",
					header.size(), header.c_str());

				res.m_header = parseHeader(header);

				if (!g_app->m_pluginMngr.HeaderPluginHandle(&res.m_header)) return;

				headerParsed = true;

				// ����������λ�ú��λ�ü�ȥ���λ��
				p += 4;
				res.m_len -= p - res.m_body;

				char *tmp = new char[res.m_len + sizeof(res.m_body[0])];

				// �˴���Ȼ����˿��ַ������ٴν����ǰ��ִ�ʵ�ʰ��峤�Ⱥ�д��
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
			// ���״̬��
			header.m_statusCode = atoi(strv[1].c_str());
		}
		else
		{
			// ״̬��Ϊ600�����޷���������Ӧ��
			header.m_statusCode = 600;
		}

		str = str.substr(pos + 2)��
	}

	while ((pos = str.find("\r\n", 0)) != string::npos)
	{
		// ������������
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

