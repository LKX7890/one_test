#ifndef __XLS_CONVER_XML_H__
#define __XLS_CONVER_XML_H__

#include <vector>
#include <map>
#include <cstring>
#include <windows.h>

#include "include/libxl.h"
#include "include/tinyxml2.h"

using namespace std;

enum ColType
{
	COLTYPE_CS = 0,
	COLTYPE_C = 1,
	COLTYPE_S = 2,
	COLTYPE_CS_ITEM = 3,
	COLTYPE_CS_ITEM_LIST = 4,
};

class XlsConverXml
{
public:
	XlsConverXml(const string &xls_file_path);
	~XlsConverXml();

	bool StartXlsConverXml();
	void GetXmlPathByXlsPath(const string &xls_file_path, string *xml_path);
	void FormatFilePath(string &file_path, string *format_path);
	bool CreatBookAndLoadXls();
	void InitEachSheetNameAndRout();
	void InitEachSheetColName();
	void InitEachSheetColType();
	void GetFileExtenName(const string& file_path, char exten_name[]);
	int SheetToXmlEle(tinyxml2::XMLDocument *xmlDoc, int sheet_index);
	int GetColType(const string &str);
	const char* GetColNameBySheetAndCol(int sheet_index, int col_index);
	const char* ReadCellContent(libxl::Sheet *sheet, const int32_t row, const int32_t col);
	void WriteCellTextToXml(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement* include_on, int sheet, int row, int col, int col_type);
	void GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen);

private:
	string m_xls_file_path;								// xls文件路径
	string m_xml_file_path;								// xml文件路径
	string m_xml_file_name;								// xml文件名称
	int m_row_begin_read;								// 配置从第几行开始(读表规则)
	libxl::Book* m_xls;									// 加载成book的xls文件
	map<int, const char*> m_cfg_sheet_map;				// 每个表名称
	map<int, map<int, int> > m_col_type_map;			// 每列类型
	map<int, map<int, const char*> > m_col_name_map;	// 每个表的每一列字段
};
#endif