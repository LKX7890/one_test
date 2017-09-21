#ifndef __PLUGIN_H__
#define __PLUGIN_H__

class Plugin
{
public:
	virtual ~Plugin(void){}

	virtual bool init(WebCrawler *app) = 0;
	virtual bool handler(void *arg) = 0;
};
#endif