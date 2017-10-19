#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <io.h>
#include <direct.h> 
#include "libxl.h"
#include "tinyxml2.h"

const char *formatErrorMsg_NoSeparateRows = "sheet format error:no separate rows!";
const char *formatErrorMsg_GroupNameNull = "sheet format error:group name null!";
const char *formatErrorMsg_CellValueNull = "cell format error:cell value null!";

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

int32_t ExcelToXml(const char *excelFilePath, const char *xmlFilePath);
int32_t ExcelToXmls(const char *excelFileDirectory, const char *xmlFileDirectory);
int32_t BookToXmlDoc(libxl::Book* book, tinyxml2::XMLDocument *xmlDoc);
int32_t SheetToXmlEle(libxl::Sheet *sheet, tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement *ele);
void GetPathExtensionName(const char *filePath, char ext[], const uint32_t extLen);
bool IsConfigSheet(const char *sheetName);
bool IsCellEmpty(int32_t cellType);
const char* ReadCellContent(libxl::Sheet *sheet, const int32_t row, const int32_t col);
void GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen);
void GetFilesFromDirectory(std::vector<std::string> &files, const std::string &directoryPath);
int32_t CalculateXmlFilePath(const char *excelFileDirectory, const char *xmlFileDirectory, const char *excelFilePath, char xmlFilePath[], const int32_t xmlFilePathMaxLen);
int32_t CreateDirectory(const char *directoryPath);

int32_t ExcelToXml(const char *excelFilePath, const char *xmlFilePath)
{
	libxl::Book* book = NULL;
	char ext[enmFileExtensionLength] = { 0 };
	GetPathExtensionName(excelFilePath, ext, enmFileExtensionLength);
	if (strcmp(ext, ".xls") == 0)
	{
		book = xlCreateBook();
	}
	else if (strcmp(ext, ".xlsx") == 0)
	{
		book = xlCreateXMLBook();
	}
	else
	{
		return enmErrorDef_NotExcelFile;
	}

	if (!book)
	{
		return enmErrorDef_CreateBookFailed;
	}

	if (!book->load(excelFilePath))
	{
		return enmErrorDef_LoadExcelFileFailed;
	}

	tinyxml2::XMLDocument xmlDoc;
	int32_t ret = BookToXmlDoc(book, &xmlDoc);
	if (ret != enmErrorDef_OK)
	{
		return ret;
	}
	if (xmlDoc.SaveFile(xmlFilePath) != tinyxml2::XML_SUCCESS)
	{
		return enmErrorDef_SaveXmlFileFailed;
	}
	book->release();
	return enmErrorDef_OK;
}

int32_t BookToXmlDoc(libxl::Book* book, tinyxml2::XMLDocument *xmlDoc)
{
	// 写入表格声明
	tinyxml2::XMLDeclaration *declaration = xmlDoc->NewDeclaration("version=\"1.0\" encoding=\"utf-8\"");
	xmlDoc->LinkEndChild(declaration);

	for (int32_t i = 0; i < book->sheetCount(); ++i)
	{
		if (!IsConfigSheet(book->getSheet(i)->name())){ continue; }

		// 写入子节点
		tinyxml2::XMLElement *ele = xmlDoc->NewElement(book->getSheet(i)->name() + 1);
		xmlDoc->LinkEndChild(ele);

		// printf("%s\n", book->getSheet(i)->name());
		int32_t ret = SheetToXmlEle(book->getSheet(i), xmlDoc, ele);
		if (ret != enmErrorDef_OK)
		{
			return ret;
		}
	}
	return enmErrorDef_OK;
}

int32_t SheetToXmlEle(libxl::Sheet *sheet, tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement *ele)
{
	tinyxml2::XMLElement *group = NULL;
	static std::vector<std::string> titles;
	titles.clear();
	for (int32_t row = 0; row < sheet->lastRow(); ++row)
	{
		// 获取该行前两个单元格
		int32_t firstCellType = sheet->cellType(row, 0);
		int32_t secondCellType = sheet->cellType(row, 1);
		// 如果都是空白,表明是空白行
		if (IsCellEmpty(firstCellType) && IsCellEmpty(secondCellType))
		{
			titles.clear();
			continue;
		}
		// 如果都不是空白,表明是标题行,获取标题
		else if (!IsCellEmpty(firstCellType) && !IsCellEmpty(secondCellType))
		{
			do 
			{
				// 标题列不为空,表明没有分隔,有问题
				if (!titles.empty())
				{
					group = xmlDoc->NewElement(formatErrorMsg_NoSeparateRows);
					break;
				}
				const char *firstCell = ReadCellContent(sheet, row, 0);
				if (NULL == firstCell)
				{
					group = xmlDoc->NewElement(formatErrorMsg_GroupNameNull);
					break;
				}
				group = xmlDoc->NewElement(firstCell);
				for (int32_t col = 1; col <= sheet->lastCol(); ++col)
				{
					const char *text = ReadCellContent(sheet, row, col);
					if (text == NULL || _stricmp(text, "EOF") == 0)
					{
						break;
					}
					titles.push_back(text);
				}
			} while (0);
			ele->LinkEndChild(group); // 写入标题第一行的内容
		}
		// 第一个为空,第二个不为空,表示是数据列
		else if (IsCellEmpty(firstCellType) && !IsCellEmpty(secondCellType))
		{
			tinyxml2::XMLElement *cfg = xmlDoc->NewElement("cfg");
			group->LinkEndChild(cfg);
			for (uint32_t col = 0; col < titles.size(); ++col)
			{
				const char *text = ReadCellContent(sheet, row, col + 1);
				if (NULL == text)
				{
					text = formatErrorMsg_CellValueNull;
				}
				cfg->SetAttribute(titles[col].c_str(), text);
			}
		}
	}
	return enmErrorDef_OK;
}

void GetPathExtensionName(const char *filePath, char ext[], const uint32_t extLen)
{
	int32_t len = strlen(filePath), lastSep = 0, m = 0;
	for (lastSep = len - 1; lastSep >= 0; --lastSep)
	{
		if (filePath[lastSep] == '.' || filePath[lastSep] == '\\' || filePath[lastSep] == '/')
		{
			break;
		}
	}
	if (lastSep < 0)
	{
		lastSep = 0;
	}
	for (; lastSep < len; ++lastSep)
	{
		ext[m++] = filePath[lastSep];
	}
	ext[m] = '\0';
}

bool IsConfigSheet(const char *sheetName)
{
	if (NULL == sheetName)
	{
		return false;
	}
	return sheetName[0] == '_';
}

bool IsCellEmpty(int32_t cellType)
{
	return (cellType == libxl::CELLTYPE_BLANK) || (cellType == libxl::CELLTYPE_EMPTY);
}

const char* ReadCellContent(libxl::Sheet *sheet, const int32_t row, const int32_t col)
{
	int32_t cellType = sheet->cellType(row, col);
	switch (cellType)
	{
	case libxl::CELLTYPE_STRING:
		do 
		{
			const char *text = sheet->readStr(row, col);
			// 先判断是不是全是acsii字符
			bool isAscii = true;
			int32_t textLen = strlen(text);
			for (int32_t i = 0; i < textLen; ++i)
			{
				if (!isascii(text[i]))
				{
					isAscii = false;
					break;
				}
			}
			// 如果是,则不用转换格式
			if (isAscii)
			{
				return text;
			}
			// 否则,需要转成UTF8格式
			else
			{
				const int32_t textUtf8StrLen = 1024 * 5;
				static char textUtf8Str[textUtf8StrLen];
				static WCHAR wcharTemp[textUtf8StrLen];
				GBKToUTF8(text, wcharTemp, textUtf8Str, textUtf8StrLen);
				return textUtf8Str;
			}
		} while (0);
	case libxl::CELLTYPE_NUMBER:
		do 
		{
			const int32_t doubleStrLen = 32;
			static char doubleStr[doubleStrLen];
			sprintf_s(doubleStr, doubleStrLen, "%f", sheet->readNum(row, col));
			for (int32_t i = strlen(doubleStr) - 1; i >= 0; --i)
			{
				if (doubleStr[i] > '0' && doubleStr[i] <= '9')
				{
					break;
				}
				else if ((doubleStr[i] == '0' || doubleStr[i] == '.') && (i != 0))
				{
					doubleStr[i] = 0;
				}
			}
			return doubleStr;
		} while (0);
	default:
		break;
	}
	return NULL;
}

void GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen)
{
	int32_t len = MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, wcharTmp, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, utf8Buf, len, NULL, NULL);
}

void GetFilesFromDirectory(std::vector<std::string> &files, const std::string &directoryPath)
{
	struct _finddata_t fileinfo;  // 文件信息
	long hFile = 0;
	char tmpPath[MAX_PATH] = { 0 };

	// 设置查找所有文件
	sprintf_s(tmpPath, "%s\\*", directoryPath.c_str());

	// 查找成功返回句柄， 信息保存在fileinfo中
	if ((hFile = _findfirst(tmpPath, &fileinfo)) == -1){ return; }  
	do
	{
		// attrib 文件属性 unsigned类型，用位保存信息； 如果是文件递归查找
		if ((fileinfo.attrib &  _A_SUBDIR))
		{
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			{
				sprintf_s(tmpPath, "%s\\%s", directoryPath.c_str(), fileinfo.name);
				GetFilesFromDirectory(files, tmpPath);
			}
		}
		else
		{
			// 找到文件放入文件路径vector中
			sprintf_s(tmpPath, "%s\\%s", directoryPath.c_str(), fileinfo.name);
			files.push_back(tmpPath);
		}
	} while (_findnext(hFile, &fileinfo) == 0);	// 搜索文件路径匹配的下一个实例
	_findclose(hFile);
}

int32_t ExcelToXmls(const char *excelFileDirectory, const char *xmlFileDirectory)
{
	char xmlFilePath[MAX_PATH];
	std::vector<std::string> files;
	GetFilesFromDirectory(files, excelFileDirectory);
	for (uint32_t i = 0; i < files.size(); ++i)
	{
		printf("%s\n", files[i].c_str());
		int32_t ret = CalculateXmlFilePath(excelFileDirectory, xmlFileDirectory, files[i].c_str(), xmlFilePath, MAX_PATH);
		if (ret != enmErrorDef_OK){ return ret; }

		ret = CreateDirectory(xmlFilePath);
		if (ret != enmErrorDef_OK){ return ret; }

		ExcelToXml(files[i].c_str(), xmlFilePath);
		printf("\t%s\n", xmlFilePath);
	}
	return enmErrorDef_OK;
}

int32_t CalculateXmlFilePath(const char *excelFileDirectory, const char *xmlFileDirectory, const char *excelFilePath, char xmlFilePath[], const int32_t xmlFilePathMaxLen)
{
	int32_t excelFileDirectoryLen = strlen(excelFileDirectory);
	int32_t excelFilePathLen = strlen(excelFilePath);

	if (excelFileDirectoryLen > excelFilePathLen){ return enmErrorDef_CalculateXmlFilePathError; }

	// 计算xml文件夹路径+excel路径+长度
	sprintf_s(xmlFilePath, xmlFilePathMaxLen, "%s%s", xmlFileDirectory, excelFilePath + excelFileDirectoryLen);
	int32_t xmlFilePathLen = strlen(xmlFilePath);
	for (int32_t i = xmlFilePathLen - 1; i >= 0; --i)
	{
		if (xmlFilePath[i] == '.')
		{
			xmlFilePath[i + 1] = 'x';
			xmlFilePath[i + 2] = 'm';
			xmlFilePath[i + 3] = 'l';
			xmlFilePath[i + 4] = 0;
			break;
		}
	}
	return enmErrorDef_OK;
}

int32_t main()
{
	for (int32_t i = 0; i < 1; ++i)
	{
		ExcelToXmls("excel", "xml");
	}
	system("pause");
	return 0;
}

int32_t CreateDirectory(const char *directoryPath)
{
	int32_t dirPathLen = strlen(directoryPath);
	if (dirPathLen > MAX_PATH)
	{
		return enmErrorDef_MakeXmlFilePathError;
	}

	char tmpDirPath[MAX_PATH] = { 0 };
	for (int32_t i = 0; i < dirPathLen; ++i)
	{
		tmpDirPath[i] = directoryPath[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			// 判断文件权限
			if (_access(tmpDirPath, 0) != 0)
			{
				// 创建文件夹
				int32_t ret = _mkdir(tmpDirPath);
				if (ret != 0)
				{
					return enmErrorDef_MakeXmlFilePathError;
				}
			}
		}
	}
	return enmErrorDef_OK;
}
