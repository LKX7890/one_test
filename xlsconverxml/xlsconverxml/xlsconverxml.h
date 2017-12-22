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

struct ColAttr
{
	ColAttr() : col_type(0) {}

	int col_type;						// 列类型
	std::string col_name;				// 列字段名称
};

struct SheetDefineInfo
{
	SheetDefineInfo() : sheet_index(0) {}

	int sheet_index;					// 表索引
	std::string sheet_name;				// 表名称
	std::map<int, ColAttr> col_info_list;	// 单元格信息
};

struct ItemInfo
{
	ItemInfo(){}

	std::string item_id;
	std::string item_num;
	std::string is_bind;
};

class XlsConverXml
{
public:
	XlsConverXml(const string &xls_file_path);
	~XlsConverXml();

	bool StartXlsConverXml();
	void GetXmlPathByXlsPath(const string &xls_file_path, string *xml_path);
	void FormatFilePath(const string &file_path, string *format_path);
	bool CreatBookAndLoadXls();
	bool ReadSheetDefineInfo();
	void GetFileTypeName(const string& file_path, char exten_name[]);
	int SheetConverXmlFile(tinyxml2::XMLDocument *xmlDoc, const int sheet_index);
	int GetColType(const string &str);
	std::string GetColNameBySheetAndCol(const int sheet_index, const int col_index);
	const char* ReadCellContent(libxl::Sheet *sheet, const int32_t row, const int32_t col);
	void WriteCellTextToXmlFile(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement* include_on, int sheet, int row, int col, int col_type);
	void GetItemInfoFromCell(int cell_type, std::string cell_val, std::vector<ItemInfo>& item_list);
	void GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen);
	void StrSplit(std::string& str, std::string& delim, std::vector<std::string>& ret);
	void OnXmlWriteItemInfoList(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement* include_on, const std::string cell_name, const int cell_type, const std::string cell_val);

private:
	string m_xls_file_path;								// xls文件路径
	string m_xml_file_path;								// xml文件路径
	libxl::Book* m_xls;									// 加载成book的xls文件
	std::map<int, SheetDefineInfo> m_sheet_info_list;		// 表信息
};
#endif