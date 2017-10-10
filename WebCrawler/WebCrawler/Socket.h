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

	bool sendRequest(void);						// 发送协议请求
	bool recvResponse(void);					// 接收协议响应
	int  sockfd(void) const;					// 获取套接字描述符
private:

	HttpHeader parseHeader(string str) const;    // 超文本传输协议响应
	int m_sockfd;								 // 套接字描述符
	DnsUrl m_dnsUrl;							 
};
#endif