#ifndef  __SAVEHTMLTOFILE_H__
#define  __SAVEHTMLTOFILE_H__

class SaveHTMLToFile : public Plugin
{
private:

	bool Init(WebCrawler *app);
	bool Handler(void *arg);
};
#endif
