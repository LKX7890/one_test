#include "SendThread.h"
#include "Log.h"



void * SendThread::ThreadHandlerFunc(void)
{
	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�����߳̿�ʼ");

	for (;;)
	{
		g_app->StartJob();
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�����߳���ֹ");

	return NULL;
}
