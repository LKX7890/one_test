#ifndef __THREAD_H__
#define __THREAD_H__

class Thread
{
public:

	virtual ~Thread(void){}
	void start(void);

private:

	static void *run(void *arg);
	virtual void *run(void) = 0;

	pthread_t m_tid;
};
#endif