#ifndef __HTTP_HEADER_H__
#define __HTTP_HEADER_H__

#include "common.h"
#include "Url.h"

class HttpHeader
{
public:

	int m_statusCode;			// 状态码
	string m_contentType;		// 内容类型
};

class HttpResponse
{
public:

	HttpResponse(DnsUrl const& dnsUrl) :m_dnsUrl(dnsUrl), m_body(NULL), m_len(0){}
	~HttpResponse(void)
	{
		if (m_body)
		{
			free(m_body);
			m_body = NULL;
		}

		m_len = 0;
	}

	DnsUrl m_dnsUrl;		// 资源定位符
	HttpHeader m_header;	// 协议响应包头
	char *m_body;			// 包体指针
	size_t m_len;			// 包体长度
};
#endif