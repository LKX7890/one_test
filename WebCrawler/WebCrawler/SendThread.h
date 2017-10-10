#ifndef __SENDTHREAD_H__
#define __SENDTHREAD_H__

#include "Thread.h"

class Socket;

class SendThread : public Thread
{
private:

	void *ThreadHandlerFunc(void);
};
#endif