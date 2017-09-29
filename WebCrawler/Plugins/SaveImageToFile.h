#ifndef __SAVEIMAGETOFILE_H__
#define __SAVEIMAGETOFILE_H__

class SaveImageToFile : public Plugin
{
private:

	bool Init(WebCrawler *app);
	bool Handler(void *arg);
};
#endif
