#include "SaveHTMLtoFile.h"
#include "WebCrawler\WebCrawler.h"

SaveHTMLToFile g_pluginSaveHTMLToFile;
WebCrawler *g_app;

bool SaveHTMLToFile::Init(WebCrawler *app)
{
	(g_app = app)->m_pluginMngr.RegisterHtmlPlugin(this);
	return true;
}

bool SaveHTMLToFile::Handler(void *arg)
{
	HttpResponse *res = static_cast<HttpResponse*>(arg);

	if (res->m_header.m_contentType.find("text/html", 0) == string::npos)
	{
		return false;
	}

	string filename = res->m_dnsUrl.toFilename();

	ofstream ofs(filename.c_str(), ios::binary);
	if (!ofs)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "���ļ�%sʧ�ܣ�%s", filename.c_str(), strerror(errno));
		return false;
	}

	if (!ofs.write(res->m_body, res->len))
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "д���ļ�%sʧ�ܣ�%s", filename.c_str(), strerror(errno));

		ofs.close();
		unlink(filename.c_str());
		return false;
	}

	g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�ļ�%s����ɹ�", filename.c_str());

	ofs.close();

	return true;
}
