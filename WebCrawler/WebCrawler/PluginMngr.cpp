#include "PluginMngr.h"

#include "common.h"

void PluginMngr::LoadPlugins(void)
{
	typedef vector<string>::const_iterator Iter;
	for (Iter it = g_app->m_cfg.m_loadPlugins.begin(); it != g_app->m_cfg.m_loadPlugins.end(); ++i)
	{
		string path = StrKit::StrCat(g_app->m_cfg.m_pluginsPath.c_str(), "/", it->c_str(), ".so", NULL);

		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "¼ÓÔØ\"%s\"²å¼þ", path.c_str());

		void *handle = dlopen(path.c_str(), RTLD_GLOBAL | RTLD_NOW);
		if (!handle)
		{
			g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "dlopen: %s", dlerror());
		}

		string symbol = "g_plugin";
		symbol += *it;

		Plugin *plugin = (Plugin*)dlsym(handle, symbol.c_str());

		if (!plugin)
		{
			g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "dlsym: %s", dlerror());
		}

		plugin->init(g_app);
	}
}

void PluginMngr::RegisterUrlPlugin(Plugin *plugin)
{
	m_urlPlugins.push_back(plugin);
}

void PluginMngr::RegisterHeaderPlugin(Plugin *plugin)
{
	m_headerPlugins.push_back(plugin);
}

void PluginMngr::RegisterHtmlPlugin(Plugin *plugin)
{
	m_htmlPlugins.push_back(plugin);
}

bool PluginMngr::UrlPluginHandle(void *arg) const
{
	for (vector<Plugin*>::const_iterator it = m_urlPlugins.begin(); it != m_urlPlugins.end(); ++it)
	{
		if (!(*it)->handler(arg))
		{
			return false;
		}
	}

	return true;
}

bool PluginMngr::HeaderPluginHandle(void *arg) const
{
	for (vector<Plugin*>::const_iterator it = m_headerPlugins.begin(); it != m_headerPlugins.end(); ++it)
	{
		if (!(*it)->handler(arg))
		{
			return false;
		}
	}

	return true;
}

bool PluginMngr::HtmlPluginHandle(void *arg) const
{
	for (vector<Plugin*>::const_iterator it = m_htmlPlugins.begin(); it != m_htmlPlugins.end(); ++it)
	{
		if (!(*it)->handler(arg))
		{
			return false;
		}
	}

	return true;
}
