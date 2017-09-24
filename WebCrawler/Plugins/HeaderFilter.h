#ifndef __HEADERFILTER_H__
#define __HEADERFILTER_H__

class HeaderFilter : public Plugin
{
private:

	bool Init(WebCrawler *app);
	bool Handler(void *rag);
};
#endif