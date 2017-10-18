#ifndef __URLQUEUES_H__
#define __URLQUEUES_H__

#include "Url.h"
#include "common.h"

class UrlFilter;

class UrlQueues
{
public:

	UrlQueues(UrlFilter &filter);
	~UrlQueues();

	void pushRawUrl(RawUrl const &rawUrl);
	RawUrl popRawUrl(void);
	size_t sizeRawUrl(void) const;
	bool emptyRawUrl(void) const;
	bool fullRawUrl(void) const;
	void clearRawUrl(void);

	void pushDnsUrl(DnsUrl const *dnsUrl);
	DnsUrl popDnsUrl(void);
	size_t sizeDnsUrl(void)const;
	bool emptyDnsUrl(void) const;
	bool fullDnsUrl(void) const;
	void clearDnsUrl(void);

	void extractUrl(char const *html, DnsUrl const &dnsUrl);		// 从超文本标记语言页面内容中抽取Url

private:

	static bool isBinary(string const &strUrl);

	UrlFilter &m_fileter;
	list<RawUrl>m_rawUrlQueue;										// 原始URL列表
	mutable pthread_mutex_t m_rawUrlMutex;
	pthread_cond_t m_rawUrlNoFull;
	pthread_cond_t m_rawUrlNoEmpty;

	list<DnsUrl> m_dnsUrlQueue;										// 解析URL列表
	mutable pthread_mutex_t m_dnsUrlMutex;
	pthread_cond_t m_dnsUrlNoFull;
	pthread_cond_t m_dnsUrlNoEmpty;
};
#endif
