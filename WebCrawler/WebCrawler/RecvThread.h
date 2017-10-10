#ifndef __RECVTHREAD_H
#define __RECVTHREAD_H

#include "Thread.h"

class Socket;

class RecvThread : public Thread
{
public:

	RecvThread(Socket *socket);
	~RecvThread(void);

private:

	void *ThreadHandlerFunc(void);
	Socket *m_socket;
};
#endif