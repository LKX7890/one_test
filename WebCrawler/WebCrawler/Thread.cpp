#include "Thread.h"
#include "Log.h"



void Thread::ThreadStart(void)
{
	// 初始化线程
	pthread_attr_t attr;								
	pthread_attr_init(&attr);

	// 设置线程大小
	pthread_attr_setstacksize(&attr, 1024 * 1024);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	// 线程分离

	// 产生线程
	int error = pthread_create(&m_tid, &attr, ThreadRun, this);
	if (error)
	{
		g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "pthread_create: %s", strerror(error));
	}

	// 销毁线程
	pthread_attr_destroy(&attr);
}

void * Thread::ThreadRun(void *arg)
{
	return static_cast<Thread*>(arg)->ThreadHandlerFunc();
}
