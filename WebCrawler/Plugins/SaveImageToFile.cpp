#include "SaveImageToFile.h"



bool SaveImageToFile::Init(WebCrawler *app)
{
	(g_app = app)->m_pluginMngr.RegisterHtmlPlugin(this);
	return true;
}

bool SaveImageToFile::Handler(void *arg)
{
	HttpResponse *res = static_cast<HttpResponse*>(arg);

	if (res->m_header.m_contentType.find("text/html", 0) != string::npos)
	{
		regex_t ex;		// 正则表达式
		
		// 编译正则表达式
		int error = regcomp(&ex, "<img[^>]*str=\"\\s*\\([^>\"]*\\)\\s*\"", 0);
		if (error)
		{
			char errInfo[1024];
			regerror(error, &ex, errInfo, sizeof(errInfo) / sizeof(errInfo[0]));
			g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "REGCOMP:%s", errInfo);
		}

		char const *html = res->m_body;

		// 匹配集合
		regmatch_t match[2];

		while (regexec(&ex, html, sizeof(match) / sizeof(match[0]), match, 0) != REG_NOMATCH)
		{
			html += match[1].rm_so;
			size_t len = match[1].rm_eo - match[1].rm_so;
			string strUrl(html, len);
			html += len + match[0].rm_eo - match[1].rm_eo;

			if (!res->m_dnsUrl.attachDomain(strUrl))
			{
				continue;
			}

			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "抽取到一个深度%d的统一资源定位符\"%s\"",
				res->m_dnsUrl.m_depth, strUrl.c_str());

			if (!RawUrl::FormattingRawUrl(strUrl))
			{
				g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "规格化统一资源定位符\"%s\"失败", strUrl.c_str());
				continue;
			}

			g_app->m_urlQueues.pushRawUrl(RawUrl(strUrl, RawUrl::ETYPE_IMAGE, res->m_dnsUrl.m_depth));
		}

		regfree(&ex);
	}

	else if (res->m_header.m_contentType.find("image", 0) != string::npos)
	{
		string filename = res->m_dnsUrl.toFilename();

		ofstream ofs(filename.c_str(), ios::binary);

		if (!ofs)
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "打开文件%s失败：%s", filename.c_str(), strerror(error));
			return;
		}

		// 包体就是资源数据
		if (!ofs.write(res->m_body, res->m_len))
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "写入文件%s失败：%s", fiename.c_str(), strerror(errno));

			ofs.close();

			unlink(filename.c_str());

			return false;
		}


		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "文件%s保存成功"， filename.c_str());

		ofs.close();
	}

	return true;
}
