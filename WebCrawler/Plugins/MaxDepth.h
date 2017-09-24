#ifndef __MAXDEPTH_H__
#define __MAXDEPTH_H__

#include "Plugin.h"

class MaxDepth : public Plugin
{
private:

	bool Init(WebCrawler *app);
	bool Handler(void *arg);
};
#endif