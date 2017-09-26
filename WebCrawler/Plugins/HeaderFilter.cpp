#include "HeaderFilter.h"



bool HeaderFilter::Init(WebCrawler *app)
{
	(g_app = app)->m_pluginMngr.RegisterHeaderPlugin(this);
}

bool HeaderFilter::Handler(void *rag)
{
	HttpHeader *header = static_cast<HttpHeader*>(arg);

	if (header->m_statusCode < 200 || 300 <= header->m_statusCode)
	{
		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "���ı�����Э����Ӧ״̬��%d���ں�������[200, 300]֮��"��
			header->m_statusCode);

		return false;
	}

	if (header->m_contentType.find("text/html", 0) == string::npos)
	{
		vector<string>::const_iterator it;

		for (it = g_app->m_cfg.m_acceptType.begin(); it != g_app->m_cfg.m_acceptType.end() &&
			header->m_contentType.find(*it, 0) == string::npos; ++it)
		{
			if (it == g_app->m_cfg.m_acceptTyes.end())
			{
				g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__,
					"���ı�����Э����Ӧ����%s���ڿɽ��ܷ�Χ֮��"�� header->m_contentType.c_str());

				return false;
			}
		}
	}

	return true;
}
