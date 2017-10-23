#ifndef __XLS_CONVER_XML_H__
#define __XLS_CONVER_XML_H__

#include <vector>

using namespace std;

class XlsConverXml
{
public:
	XlsConverXml(const string &xls_file_path);
	~XlsConverXml();

	bool StartXlsConverXml();
	const string & GetXmlPath(const string &xls_file_path);
	const string & FormatFilePath(string &file_path);
	int BookToXmlDoc();
	int XlsToXml();

private:
	string m_xls_file_path;
	string m_cfg_file_name;
	string m_xml_file_name;
	int m_row_begin_read;
	vector<string> m_cfg_sheet_list;
};
#endif