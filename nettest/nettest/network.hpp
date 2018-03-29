#ifndef __NETWORK_HPP_
#define __NETWORK_HPP_

#include <cstdlib>
#include <WinSock2.h>  

static const int MAX_MSG_SIZE = 1024;

class NetWork
{
public:
	static NetWork* GetNetWorkInstall();

	bool NetWorkInit();
	bool NetWorkStart();
	bool NetWorkSendMsg(void* msg_data);

private:
	NetWork();
	~NetWork();

	static NetWork* m_network;

	SOCKET sockSrv;					// 用于监听的套接字
	SOCKET sockConn;				// 单个人连接的socket

	int m_port;
	char m_recvBuf[MAX_MSG_SIZE];
	char m_sendBuf[MAX_MSG_SIZE];
};


#endif
