#ifndef __PLUGINMNGR_H__
#define __PLUGINMNGR_H__

#include "common.h"

class Plugin;

class PluginMngr
{
public:

	void LoadPlugins(void);					// ���ز��

	void RegisterUrlPlugin(Plugin *plugin);
	void RegisterHeaderPlugin(Plugin *plugin);
	void RegisterHtmlPlugin(Plugin *plugin);

	bool UrlPluginHandle(void *arg) const;
	bool HeaderPluginHandle(void *arg) const;
	bool HtmlPluginHandle(void *arg) const;
private:

	vector<Plugin*> m_urlPlugins;		// url�������
	vector<Plugin*> m_headerPlugins;	// ��Ӧ��ͷ�������
	vector<Plugin*> m_htmlPlugins;		// ���ı�������Բ������
};

#endif