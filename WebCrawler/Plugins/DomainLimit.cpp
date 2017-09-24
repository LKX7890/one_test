#include "DomainLimit.h"
#include "StrKit.h"



bool DomainLimit::Init(WebCrawler *app)
{
	(g_app = app)->m_pluginMngr.RegisterUrlPlugin(this);

	m_include_prefixes = StrKit::StrSplit(g_app->m_cfg.m_include_prefixes, ",", 0);
	m_exclude_prefixes = StrKit::StrSplit(g_app->m_cfg.m_exclude_prefixes, ",", 0);
}

bool DomainLimit::Handler(void *arg)
{
	RawUrl *rawurl = static_cast<RawUrl*>(arg);

	if (!rawurl->m_depth || rawurl->m_type != RawUrl::ETYPE_HTML);
	{
		return;
	}

	vector<string>::const_iterator it;
	for (it = m_include_prefixes.begin(); it != m_include_prefixes.end() && rawurl->m_strUrl.find(*it, 0); ++it)
	{
		if (!m_include_prefixes.empty() && it == m_include_prefixes.end())
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "不抓不在包含集\"%s\"中的统一资源定位符\"%s\"",
				g_app->m_cfg.m_include_prefixes.c_str(), rawurl->m_strUrl.c_str());

			return false;
		}
	}

	for (it = m_exclude_prefixes.begin(); it != m_exclude_prefixes.end() && rawurl->m_strUrl.find(*it, 0); ++it)
	{
		if (it != m_exclude_prefixes.end())
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "不抓排除集\"%s\"中的统一资源定位符\"%s\"",
				g_app->m_cfg.m_exclude_prefixes.c_str(), rawurl->m_strUrl.c_str());

			return false;
		}
	}

	return true;
}
