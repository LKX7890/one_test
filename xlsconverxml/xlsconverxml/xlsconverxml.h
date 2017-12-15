#ifndef __XLS_CONVER_XML_H__
#define __XLS_CONVER_XML_H__

#include <vector>
#include <map>
#include <cstring>
#include <windows.h>

#include "include/libxl.h"
#include "include/tinyxml2.h"

using namespace std;

static const int ROL_BEGIN_WRITE_IN_XML = 1; // 从注释后第几行为写入xml的内容

enum ColType
{
	COLTYPE_CS = 0,
	COLTYPE_C = 1,
	COLTYPE_S = 2,
	COLTYPE_CS_ITEM = 3,
	COLTYPE_CS_ITEM_LIST = 4,
};

struct ColAttr
{
	int col_type;						// 列类型
	std::string col_name;				// 列字段名称
};

struct SheetDefineInfo
{
	int sheet_index;					// 表索引
	std::string sheet_name;				// 表名称
	std::map<int, ColAttr> col_info_list;	// 单元格信息
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
	std::map<int, SheetDefineInfo> m_sheet_info_list;		// 表信息
};
#endif