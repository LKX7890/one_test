#ifndef __THREAD_H__
#define __THREAD_H__

class Thread
{
public:

	virtual ~Thread(void){}
	void ThreadStart(void);

private:

	static void *ThreadRun(void *arg);
	virtual void *ThreadHandlerFunc(void) = 0;

	pthread_t m_tid;
};
#endif