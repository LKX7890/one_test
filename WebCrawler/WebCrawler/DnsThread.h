#ifndef DNSTHREAD_H__
#define DNSTHREAD_H__

#include "Thread.h"

class DnsThread : public Thread
{
private:

	void *run(void);
	static map<string, string> s_hosts;
};


#endif