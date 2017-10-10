#ifndef DNSTHREAD_H__
#define DNSTHREAD_H__

#include "Thread.h"
#include "common.h"

class DnsThread : public Thread
{
private:

	void *ThreadHandlerFunc(void);
	static map<string, string> s_hosts;
};


#endif