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
	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收线程开始");

	g_app->StopJob(m_socket->recvResponse());
	delete this;

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收线程终止");

	return NULL;
}
