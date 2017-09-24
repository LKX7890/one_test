#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "WebCrawler.h"

class Plugin
{
public:
	virtual ~Plugin(void){}

	virtual bool Init(WebCrawler *app) = 0;
	virtual bool Handler(void *arg) = 0;
};
#endif