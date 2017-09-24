#include "MaxDepth.h"
#include "WebCrawler.h"
#include "PluginMngr.h"
#include "common.h"

MaxDepth g_plugin_maxdepth;
WebCrawler *g_app;

bool MaxDepth::Init(WebCrawler *app)
{
	(g_app = app)->m_pluginMngr.RegisterUrlPlugin(this);

	return true;
}

bool MaxDepth::Handler(void *arg)
{
	RawUrl *rawurl = static_cast<RawUrl*>(arg);

	if (0 <= g_app->m_cfg.m_max_depth && g_app->m_cfg.m_max_depth < rawurl->m_depth)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "不抓过深(%d > %d)的统一资源定位符\"%s\"",
			rawurl->m_depth, g_app->m_cfg.m_max_depth, rawurl->m_strUrl.c_str());

		return false;
	}

	return false;
}
