#include "Url.h"



RawUrl::RawUrl(string const *strUrl, ETYPE type /*= ETYPE_HTML*/, int depth /*= 0*/)
{

}

bool RawUrl::FormattingRawUrl(string &strUrl)
{
	string strTmp = strUrl;
	StrKit::StrTrim(strTmp);

	if (strTmp.empty())
	{
		return false;
	}

	if (!strTmp.find("http://", 0))
	{
		strTmp = strTmp.substr(7);
	}
	else if (!strTmp.find("https://", 0))
	{
		strTmp = strTmp.substr(8);
	}

	if (*(strTmp.end()) - 1 == '/')
	{
		strTmp.erase(strTmp.end() - 1);
	}

	if (strTmp.size() > 128)
	{
		return false;
	}

	strUrl = strTmp;

	return true;
}

DnsUrl::DnsUrl(RawUrl const &rawUrl)
{
	string::size_type = m_strUrl.find_first_of('/');

	if (pos == string::npos)
	{
		m_domain = m_strUrl;
	}
	else
	{
		m_domain = m_strUrl.substr(0, pos);
		m_path = m_strUrl.substr(pos + 1);
	}

	if ((pos = m_domain.find_last_of(':')) == string::npos || !(m_port = atoi(m_domain.substr(pos + 1).c_str())))
	{
		m_port = 80;
	}
}

std::string DnsUrl::toFilename(void) const
{
	string filename = m_domain;

	if (!m_path.empty())
	{
		(filename += "/") += m_path;
	}

	for (string::size_type pos = 0; (pos = filename.find('/', pos)) != string::npos; ++pos)
	{
		filename.replace(pos, 1, "_");
	}

	if (m_type == ETYPE_HTML)
	{
		string::size_type pos = filename.find_last_of('.');

		if (pos == string::pos || (filename.substr(pos) != ".htm" && filename.substr(pos) != ".html"))
		{
			filename += ".html";
		}
	}

	return filename;
}

bool DnsUrl::attachDomain(string &strUrl) const
{
	if (!strUrl.find("http", 0))
	{
		return true;
	}

	if (strUrl.empty() || strUrl[0] != '/')
	{
		return false;
	}

	strUrl.insert(0, m_domain);
	return true;
}
