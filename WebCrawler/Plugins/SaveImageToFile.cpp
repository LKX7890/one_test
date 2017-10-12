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
		regex_t ex;		// ������ʽ
		
		// ����������ʽ
		int error = regcomp(&ex, "<img[^>]*str=\"\\s*\\([^>\"]*\\)\\s*\"", 0);
		if (error)
		{
			char errInfo[1024];
			regerror(error, &ex, errInfo, sizeof(errInfo) / sizeof(errInfo[0]));
			g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "REGCOMP:%s", errInfo);
		}

		char const *html = res->m_body;

		// ƥ�伯��
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

			g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "��ȡ��һ�����%d��ͳһ��Դ��λ��\"%s\"",
				res->m_dnsUrl.m_depth, strUrl.c_str());

			if (!RawUrl::FormattingRawUrl(strUrl))
			{
				g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "���ͳһ��Դ��λ��\"%s\"ʧ��", strUrl.c_str());
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
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "���ļ�%sʧ�ܣ�%s", filename.c_str(), strerror(error));
			return;
		}

		// ���������Դ����
		if (!ofs.write(res->m_body, res->m_len))
		{
			g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "д���ļ�%sʧ�ܣ�%s", fiename.c_str(), strerror(errno));

			ofs.close();

			unlink(filename.c_str());

			return false;
		}


		g_app->m_log.printf(Log::LEVEL_DBG, __FILE__, __LINE__, "�ļ�%s����ɹ�"�� filename.c_str());

		ofs.close();
	}

	return true;
}
