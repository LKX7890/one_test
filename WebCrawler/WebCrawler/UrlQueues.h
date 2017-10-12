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

	void extractUrl(char const *html, DnsUrl const &dnsUrl);		// �ӳ��ı��������ҳ�������г�ȡUrl

private:

	static bool isBinary(string const &strUrl);

	UrlFilter &m_fileter;
	list<RawUrl>m_rawUrlQueue;										// ԭʼURL�б�
	mutable pthread_mutex_t m_rawUrlMutex;
	pthread_cond_t m_rawUrlNoFull;
	pthread_cond_t m_rawUrlNoEmpty;

	list<DnsUrl> m_dnsUrlQueue;										// ����URL�б�
	mutable pthread_mutex_t m_dnsUrlMutex;
	pthread_cond_t m_dnsUrlNoFull;
	pthread_cond_t m_dnsUrlNoEmpty;
};
#endif
