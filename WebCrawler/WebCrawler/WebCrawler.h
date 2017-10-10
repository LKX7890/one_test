#ifndef __WEBCRAWLER_H__
#define __WEBCRAWLER_H__

#include "Configurator.h"
#include "PluginMngr.h"
#include "Log.h"
#include "MultiIo.h"
#include "UrlQueues.h"
#include "DnsThread.h"
#include "SendThread.h"

class WebCrawler
{
public:

	WebCrawler(UrlFilter &filter);
	~WebCrawler();

	void Init(bool daemon = false);
	void ExecMultiIO();
	void StartJob(void);
	void StopJob(bool success = true);

	Log m_log;
	Configurator m_cfg;
	MultiIo m_multiIO;
	PluginMngr m_pluginMngr;
	UrlQueues m_urlQueues;
	DnsThread m_dnsThread;
	SendThread m_sendThread;

private:

	static void Sigalrm(int signum);
	void InitDaemon(void) const;
	bool InitMaxFiles(rlim_t maxFiles)const;
	void InitSeeds(void);
	void InitDns(void);
	void InitTicker(void)const;
	void InitSend(void);

	int m_cur_jobs;
	time_t m_start;
	unsigned int m_success;
	unsigned int m_failure;
	pthreas_mutex_t m_mutex;
	pthread_cond_t m_cond;
};
#endif