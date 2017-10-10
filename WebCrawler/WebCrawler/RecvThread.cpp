#include "RecvThread.h"

class Socket;

RecvThread::RecvThread(Socket *socket) : m_socket(socket)
{

}

RecvThread::~RecvThread(void)
{
	delete m_socket;
}

void * RecvThread::ThreadHandlerFunc(void)
{
	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�����߳̿�ʼ");

	g_app->StopJob(m_socket->recvResponse());
	delete this;

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�����߳���ֹ");

	return NULL;
}
