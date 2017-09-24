#ifndef __DOMAINLIMIT_H__
#define __DOMAINLIMIT_H__

#include "Plugin.h"

class DomainLimit : public Plugin
{
private:

	bool Init(WebCrawler *app);
	bool Handler(void *arg);

	vector<string> m_include_prefixes;
	vector<string> m_exclude_prefixes;
};

#endif