#ifndef __PLUGINMNGR_H__
#define __PLUGINMNGR_H__

#include "common.h"

class Plugin;

class PluginMngr
{
public:

	void LoadPlugins(void);					// 加载插件

	void RegisterUrlPlugin(Plugin *plugin);
	void RegisterHeaderPlugin(Plugin *plugin);
	void RegisterHtmlPlugin(Plugin *plugin);

	bool UrlPluginHandle(void *arg) const;
	bool HeaderPluginHandle(void *arg) const;
	bool HtmlPluginHandle(void *arg) const;
private:

	vector<Plugin*> m_urlPlugins;		// url插件容器
	vector<Plugin*> m_headerPlugins;	// 响应包头插件容量
	vector<Plugin*> m_htmlPlugins;		// 超文本标记语言插件向量
};

#endif