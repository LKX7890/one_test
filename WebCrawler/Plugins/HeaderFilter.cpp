#include "HeaderFilter.h"



bool HeaderFilter::Init(WebCrawler *app)
{
	(g_app = app)->m_pluginMngr.RegisterHeaderPlugin(this);
}

bool HeaderFilter::Handler(void *rag)
{

}
