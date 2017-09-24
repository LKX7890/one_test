#ifndef __URL_H__
#define __URL_H__

class RawUrl
{
public:

	typedef enum tag_Type
	{
		ETYPE_HTML,
		ETYPE_IMAGE,
	}ETYPE;

	RawUrl(string const *strUrl, ETYPE type = ETYPE_HTML, int depth = 0);
	static bool normalized(string *strUrl);

	string m_strUrl;
	ETYPE m_type;
	int m_depth;
};

class DnsUrl : public RawUrl
{
public:

	explicit DnsUrl(RawUrl const &rawUrl);
	bool attachDomain(string &strUrl) const;

	string m_domain;
	string m_path;
	string m_ip;
	short m_port;
};
#endif