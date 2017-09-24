#ifndef __HTTP_HEADER_H__
#define __HTTP_HEADER_H__

#include "common.h"

class HttpHeader
{
public:

	int m_statusCode;
	string m_contentType;
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

	DnsUrl m_dnsUrl;
	HttpHeader m_header;
	char *m_body;
	size_t m_len;
};
#endif