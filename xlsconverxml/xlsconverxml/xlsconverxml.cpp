#include "xlsconverxml.h"



XlsConverXml::XlsConverXml(const string &xls_file_path) : m_row_begin_read(0)
{
	m_xls_file_path = xls_file_path;
}

XlsConverXml::~XlsConverXml()
{

}

bool XlsConverXml::StartXlsConverXml()
{
	// 获取路径
	if (m_xls_file_path.empty())
	{
		printf("%s, %s\n", __FUNCTION__, __LINE__);
		return;
	}

	// 转换
	this->XlsToXml();

	// 将获得保存路径用于保存
}

const std::string & XlsConverXml::GetXmlPath(const string &xls_file_path)
{
	if (m_xls_file_path.empty())
	{
		printf("%s, %s\n", __FUNCTION__, __LINE__);
		return;
	}

	return xls_file_path;
}

const std::string & XlsConverXml::FormatFilePath(string &file_path)
{
	if (m_xls_file_path.empty())
	{
		printf("%s, %s\n", __FUNCTION__, __LINE__);
		return;
	}
	
	string temp_path = file_path;
	string::size_type first = temp_path.find_first_not_of("\\");
	string::size_type last = temp_path.find_last_not_of("\\");
	if (first == string::npos || last == string::npos)
	{
		printf("%s, %s\n", __FUNCTION__, __LINE__);
		return;
	}

	while (first != string::npos)
	{
		string::size_type pos = first + 1;
		temp_path.insert(pos, "\\");
		first = temp_path.find_last_not_of("\\", pos + 1);
	}

	return temp_path;
}
