#pragma once

#include "include/libxl.h"
#include "tinyxml2.h"
#include <vector>

using namespace std;

static const char *formatErrorMsg_NoSeparateRows = "sheet format error:no separate rows!";
static const char *formatErrorMsg_GroupNameNull = "sheet format error:group name null!";
static const char *formatErrorMsg_CellValueNull = "cell format error:cell value null!";

enum
{
	enmFileExtensionLength = 16,
};

enum ErrorDef
{
	enmErrorDef_OK = 0,
	enmErrorDef_CreateBookFailed = 1,
	enmErrorDef_NotExcelFile = 2,
	enmErrorDef_LoadExcelFileFailed = 3,
	enmErrorDef_SaveXmlFileFailed = 4,
	enmErrorDef_CalculateXmlFilePathError = 5,
	enmErrorDef_MakeXmlFilePathError = 5,
};

class ExcelConverToXml
{
public:
	ExcelConverToXml();
	~ExcelConverToXml();

	void Start(const char *exceldir, char *xmlpath);
	int ExcelToXml(const char *excelFilePath, const char *xmlFilePath);
	int ExcelToXmls(const char *excelFileDirectory, const char *xmlFileDirectory);
	int BookToXmlDoc(libxl::Book* book, tinyxml2::XMLDocument *xmlDoc);
	int SheetToXmlEle(libxl::Sheet *sheet, tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement *ele);
	bool IsConfigSheet(const char *sheetName);
	bool IsCellEmpty(int cellType);
	const char* ReadCellContent(libxl::Sheet *sheet, const int row, const int col);
	void GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int utf8BufLen);
	void GetFilesFromDirectory(std::vector<std::string> &files, const std::string &directoryPath);
	int CalculateXmlFilePath(const char *excelFilePath, char xmlFilePath[], const int xmlFilePathMaxLen);
	int CreateDirectory(const char *directoryPath);
	void GetPathExtensionName(const char *filePath, char ext[], const uint32_t extLen);
};

