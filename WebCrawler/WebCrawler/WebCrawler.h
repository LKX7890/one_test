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
	void StartJob(void);						// 开始各种请求
	void StopJob(bool success = true);

	Log m_log;									// 日志
	Configurator m_cfg;							// 配置
	MultiIo m_multiIO;							// 多路Io
	PluginMngr m_pluginMngr;					// 插件管理器
	UrlQueues m_urlQueues;						// RawUrl队列
	DnsThread m_dnsThread;						// 解析线程
	SendThread m_sendThread;					// 发送线程

private:

	static void Sigalrm(int signum);			// 信号处理
	void InitDaemon(void) const;				// 调用进程为精灵进程
	bool InitMaxFiles(rlim_t maxFiles)const;	// 初始化最大文件描述符数
	void InitSeeds(void);						// 将种子连接压入原始资源定位符队列
	void RunDns(void);							// 启动域名解析线程
	void InitTicker(void)const;					// 启动状态定时器
	void RunSend(void);							// 启动发送线程

	int m_cur_jobs;
	time_t m_start;
	unsigned int m_success;
	unsigned int m_failure;
	pthreas_mutex_t m_mutex;					// m_urlQueue的线程互斥锁
	pthread_cond_t m_cond;						// m_urlQueue的线程互斥量
};
#endif