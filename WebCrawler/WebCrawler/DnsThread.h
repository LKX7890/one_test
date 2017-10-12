#ifndef DNSTHREAD_H__
#define DNSTHREAD_H__

#include "Thread.h"
#include "common.h"

class DnsThread : public Thread
{
private:

	void *ThreadHandlerFunc(void);			// 解析线程处理函数
	static map<string, string> s_hosts;		// 主机域名-IP map表
};


#endif