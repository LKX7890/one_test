#include "network.hpp"
#include <iostream>
#include <cstdlib>

#include "protocol.hpp"
#include "messagehandler.hpp"

#pragma comment(lib, "ws2_32.lib")

NetWork* NetWork::m_network = NULL;

NetWork::NetWork()
{
	m_port = 5150;
	memset(m_recvBuf, 0, sizeof(m_recvBuf));
}

NetWork::~NetWork()
{
	closesocket(sockConn);
	closesocket(sockSrv);
	WSACleanup();
}
NetWork* NetWork::GetNetWorkInstall()
{
	if (NULL != m_network)
	{
		return m_network;
	}
	else
	{
		m_network = new NetWork;
		return m_network;
	}
}

bool NetWork::NetWorkInit()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Failed to load Winsock" << std::endl;
		return false;
	}

	//创建用于监听的套接字  
	sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_port); //1024以上的端口号  
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR)
	{
		printf("Failed bind:%d\n", WSAGetLastError());
		return false;
	}

	if (listen(sockSrv, 10) == SOCKET_ERROR)
	{
		printf("Listen failed:%d", WSAGetLastError());
		return false;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	//等待客户请求到来    
	sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
	if (sockConn == SOCKET_ERROR)
	{
		printf("Accept failed:%d", WSAGetLastError());
		return false;
	}
	printf("Accept client \n");

	this->NetWorkStart();
	return true;
}

bool NetWork::NetWorkStart()
{
	// 循环接收信息
	while (true)
	{
		memset(m_recvBuf, 0, sizeof(m_recvBuf));

		//接收数据  
		recv(sockConn, m_recvBuf, sizeof(m_recvBuf), 0);

		// 处理信息
		protocol::MsgHeader* heart = (protocol::MsgHeader*)m_recvBuf;
		int msg_type = heart->GetHeaderType();
		MessageHandler::GetMessageHandlerInstall()->OnMessageHandler(msg_type, m_recvBuf);
	}
}

bool NetWork::NetWorkSendMsg(void* msg_data)
{
	//发送数据
	protocol::MsgHeader* msgheader = (protocol::MsgHeader*)msg_data;
	int send_size = msgheader->GetMsgSize();
	std::cout << "send_size:" << send_size << std::endl;
	memcpy(m_sendBuf, msg_data, sizeof(m_sendBuf));
	int iSend = send(sockConn, (const char*)m_sendBuf, send_size, 0);
	std::cout << "send_size:" << iSend << std::endl;
	if (iSend == SOCKET_ERROR)
	{
		printf("send failed");
		return false;
	}

	return true;
}



