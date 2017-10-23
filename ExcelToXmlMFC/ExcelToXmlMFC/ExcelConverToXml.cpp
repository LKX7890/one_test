#include "stdafx.h"
#include "ExcelConverToXml.h"
#include <io.h>
#include <direct.h> 


ExcelConverToXml::ExcelConverToXml()
{
}


ExcelConverToXml::~ExcelConverToXml()
{
}

void ExcelConverToXml::Start(const char *excelpath, char *xmlpath)
{
	char xmlFilePath[] = { 0 };
	char excelpath_arr[] = { 0 };

	size_t excelpathlen = strlen(excelpath);

	for (int i = 0; i < excelpathlen; ++i)
	{
		excelpath_arr[i] = *(excelpath++);
	}
	excelpath_arr[excelpathlen] = '\0';

	int32_t ret = CalculateXmlFilePath(excelpath_arr, xmlFilePath, MAX_PATH);
	if (ret != enmErrorDef_OK){ return; }

	// ����xml�ļ�Ŀ¼
	ret = CreateDirectory(xmlFilePath);
	if (ret != enmErrorDef_OK){ return; }
	xmlpath = xmlFilePath;

	this->ExcelToXml(excelpath, xmlFilePath);

	
}

int ExcelConverToXml::ExcelToXml(const char *excelFilePath, const char *xmlFilePath)
{
	libxl::Book* book = NULL;
	char ext[enmFileExtensionLength] = { 0 };
	this->GetPathExtensionName(excelFilePath, ext, enmFileExtensionLength);
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

	// ����xls�ļ�
	if (!book->load(excelFilePath))
	{
		return enmErrorDef_LoadExcelFileFailed;
	}

	tinyxml2::XMLDocument xmlDoc;
	int32_t ret = BookToXmlDoc(book, &xmlDoc);	// xls�ļ�ת��ΪxmlDoc�ļ�
	if (ret != enmErrorDef_OK)
	{
		return ret;
	}

	// ����·�����ִ��������ļ�
	if (xmlDoc.SaveFile(xmlFilePath) != tinyxml2::XML_SUCCESS)
	{
		return enmErrorDef_SaveXmlFileFailed;
	}
	book->release();
	return enmErrorDef_OK;
}

int ExcelConverToXml::BookToXmlDoc(libxl::Book* book, tinyxml2::XMLDocument *xmlDoc)
{
	// д��������
	tinyxml2::XMLDeclaration *declaration = xmlDoc->NewDeclaration("version=\"1.0\" encoding=\"utf-8\"");
	xmlDoc->LinkEndChild(declaration);

	// ѭ�����
	for (int32_t i = 0; i < book->sheetCount(); ++i)
	{
		if (!IsConfigSheet(book->getSheet(i)->name())){ continue; }

		// ��ȡ�������
		tinyxml2::XMLElement *ele = xmlDoc->NewElement(book->getSheet(i)->name() + 1);
		xmlDoc->LinkEndChild(ele);

		// ���ת��xml�ļ�
		// printf("%s\n", book->getSheet(i)->name());
		int32_t ret = SheetToXmlEle(book->getSheet(i), xmlDoc, ele);
		if (ret != enmErrorDef_OK)
		{
			return ret;
		}
	}
	return enmErrorDef_OK;
}

int ExcelConverToXml::SheetToXmlEle(libxl::Sheet *sheet, tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement *ele)
{
	tinyxml2::XMLElement *group = NULL;		// ???

	static std::vector<std::string> titles;
	titles.clear();
	for (int32_t row = 0; row < sheet->lastRow(); ++row)
	{
		// ��ȡ����ǰ������Ԫ��
		int32_t firstCellType = sheet->cellType(row, 0);
		int32_t secondCellType = sheet->cellType(row, 1);

		// ������ǿհ�,�����ǿհ���
		if (IsCellEmpty(firstCellType) && IsCellEmpty(secondCellType))
		{
			titles.clear();
			continue;
		}
		// ��������ǿհ�,�����Ǳ�����,��ȡ����
		else if (!IsCellEmpty(firstCellType) && !IsCellEmpty(secondCellType))
		{
			do
			{
				// �����в�Ϊ��,����û�зָ�,������
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

				// ����һ��д��Ľڵ�
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
			ele->LinkEndChild(group); // д��ڵ��ǩ
		}
		// ��һ��Ϊ��,�ڶ�����Ϊ��,��ʾ��������
		else if (IsCellEmpty(firstCellType) && !IsCellEmpty(secondCellType))
		{
			tinyxml2::XMLElement *cfg = xmlDoc->NewElement("cfg");
			group->LinkEndChild(cfg);	// д��ڵ��ǩ

			// ���ݱ�������Ԫ�񳤶ȶ�
			for (uint32_t col = 0; col < titles.size(); ++col)
			{
				const char *text = ReadCellContent(sheet, row, col + 1);
				if (NULL == text)
				{
					text = formatErrorMsg_CellValueNull;
				}

				// ��Ԫ���ֵ��ָ���ڵ�����ֵ
				cfg->SetAttribute(titles[col].c_str(), text);
			}
		}
	}
	return enmErrorDef_OK;
}

bool ExcelConverToXml::IsConfigSheet(const char *sheetName)
{
	if (NULL == sheetName)
	{
		return false;
	}
	return sheetName[0] == '_';
}

bool ExcelConverToXml::IsCellEmpty(int32_t cellType)
{
	return (cellType == libxl::CELLTYPE_BLANK) || (cellType == libxl::CELLTYPE_EMPTY);
}

const char* ExcelConverToXml::ReadCellContent(libxl::Sheet *sheet, const int32_t row, const int32_t col)
{
	int32_t cellType = sheet->cellType(row, col);
	switch (cellType)
	{
	case libxl::CELLTYPE_STRING:
		do
		{
			const char *text = sheet->readStr(row, col);
			// ���ж��ǲ���ȫ��acsii�ַ�
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
			// �����,����ת����ʽ
			if (isAscii)
			{
				return text;
			}
			// ����,��Ҫת��UTF8��ʽ
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

void ExcelConverToXml::GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen)
{
	int32_t len = MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, NULL, 0); // ת�����ֽ��ַ�����unicode�ַ��������ַ�����
	MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, wcharTmp, len);		 // ת��
	len = WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, NULL, 0, NULL, NULL); // unicode�ַ���ת�����ֽ��ַ��������ַ�����
	WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, utf8Buf, len, NULL, NULL);  // ת��
}

void ExcelConverToXml::GetFilesFromDirectory(std::vector<std::string> &files, const std::string &directoryPath)
{
	struct _finddata_t fileinfo;  // �ļ���Ϣ
	long hFile = 0;
	char tmpPath[MAX_PATH] = { 0 };

	// ���ò���Ŀ¼�������ļ��ַ���
	sprintf_s(tmpPath, "%s\\*", directoryPath.c_str());

	// �����ļ���Ŀ¼�������ļ������ҳɹ����ؾ���� ��Ϣ������fileinfo��
	if ((hFile = _findfirst(tmpPath, &fileinfo)) == -1){ return; }
	do
	{
		// attrib �ļ����� unsigned���ͣ���λ������Ϣ�� ������ļ��ݹ����
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
			// �ҵ��ļ������ļ�·��vector��
			sprintf_s(tmpPath, "%s\\%s", directoryPath.c_str(), fileinfo.name);
			files.push_back(tmpPath);
		}
	} while (_findnext(hFile, &fileinfo) == 0);	// ������һ���ļ�
	_findclose(hFile);
}

int ExcelConverToXml::ExcelToXmls(const char *excelFileDirectory, const char *xmlFileDirectory)
{
	//char xmlFilePath[MAX_PATH];
	//std::vector<std::string> files;
	//GetFilesFromDirectory(files, excelFileDirectory);	// ����ĳ�ļ�Ŀ¼�µ��ļ����ļ�·��ѹ��files��

	//for (uint32_t i = 0; i < files.size(); ++i)	// Ŀ¼������ļ�ת��
	//{
	//	// ��ȡ�ļ�·��������
	//	printf("%s\n", files[i].c_str());
	//	int32_t ret = CalculateXmlFilePath(excelFileDirectory, xmlFileDirectory, files[i].c_str(), xmlFilePath, MAX_PATH);
	//	if (ret != enmErrorDef_OK){ return ret; }

	//	// ����xml�ļ�Ŀ¼
	//	ret = CreateDirectory(xmlFilePath);
	//	if (ret != enmErrorDef_OK){ return ret; }

	//	ExcelToXml(files[i].c_str(), xmlFilePath);
	//	printf("\t%s\n", xmlFilePath);
	//}
	return enmErrorDef_OK;
}

// ����xml�ļ�·������
int ExcelConverToXml::CalculateXmlFilePath(char excelFilePath[], char xmlFilePath[], const int32_t xmlFilePathMaxLen)
{
	// ����xml�ļ�·����ʽ
	sprintf_s(xmlFilePath, xmlFilePathMaxLen, "%s", excelFilePath);
	int32_t xmlFilePathLen = strlen(xmlFilePath);
	for (int32_t i = xmlFilePathLen - 1; i >= 0; --i)
	{
		// ����xls���ֲ���xml�ļ�����
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

int ExcelConverToXml::CreateDirectory(const char *directoryPath)
{
	int32_t dirPathLen = strlen(directoryPath);

	if (dirPathLen > MAX_PATH)
	{
		return enmErrorDef_MakeXmlFilePathError;
	}

	char tmpDirPath[MAX_PATH] = { 0 };
	for (int32_t i = 0; i < dirPathLen; ++i)
	{
		tmpDirPath[i] = *(directoryPath++);
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			// �ж��ļ�Ȩ��
			if (_access(tmpDirPath, 0) != 0)
			{
				// �����ļ���
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

void ExcelConverToXml::GetPathExtensionName(const char *filePath, char ext[], const uint32_t extLen)
{
	int32_t len = strlen(filePath), lastSep = 0, m = 0;
	char filepath_arr[] = { 0 };
	for (int i = 0; i < len; ++i)
	{
		filepath_arr[i] = *(filePath++);
	}

	for (lastSep = len - 1; lastSep >= 0; --lastSep)
	{
		if (filepath_arr[lastSep] == '.' || filepath_arr[lastSep] == '\\' || filepath_arr[lastSep] == '/')
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
		ext[m++] = filepath_arr[lastSep];
	}
	ext[m] = '\0';
}

