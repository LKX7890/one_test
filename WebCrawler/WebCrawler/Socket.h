#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "common.h"
#include "Url.h"
#include "HttpHeader.h"

class Socket
{
public:

	Socket(DnsUrl const &dnsUrl);
	~Socket(void);

	bool sendRequest(void);						// ����Э������
	bool recvResponse(void);					// ����Э����Ӧ
	int  sockfd(void) const;					// ��ȡ�׽���������
private:

	HttpHeader parseHeader(string str) const;    // ���ı�����Э����Ӧ
	int m_sockfd;								 // �׽���������
	DnsUrl m_dnsUrl;							 
};
#endif