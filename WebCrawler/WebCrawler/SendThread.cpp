#include "SendThread.h"



void * SendThread::run(void)
{
	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "发送线程开始");

	for (;;)
	{
		g_app->StartJob();
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "接收线程终止");

	return NULL;
}
