#ifndef __WEBCRAWLER_H__
#define __WEBCRAWLER_H__

class WebCrawler
{
public:

	Log m_log;
	Configurator m_cfg;

private:
	static void sigalrm(int signum);
	void initDaemon(void) const;
	bool initMaxFiles(rlim_t maxFiles)const;
};
#endif