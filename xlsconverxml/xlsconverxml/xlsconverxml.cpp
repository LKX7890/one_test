#include "xlsconverxml.h"


XlsConverXml::XlsConverXml(const string &xls_file_path) : m_row_begin_read(0), m_xls(NULL)
{
	m_xls_file_path = xls_file_path;
	this->GetXmlPathByXlsPath(xls_file_path, &m_xml_file_path);
}

XlsConverXml::~XlsConverXml()
{

}

bool XlsConverXml::StartXlsConverXml()
{
	// ��ȡ·��
	if (m_xls_file_path.empty())
	{
		printf("%s, %d\n", __FUNCTION__, __LINE__);
		return false;
	}

	// ת��
	bool ret = this->CreatBookAndLoadXls();
	if (!ret) return false;

	this->InitEachSheetNameAndRout();
	this->InitEachSheetColType();
	this->InitEachSheetColName();

	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLDeclaration *declaration = xmlDoc.NewDeclaration("version=\"1.0\" encoding=\"utf-8\"");
	xmlDoc.LinkEndChild(declaration);

	tinyxml2::XMLElement *config = xmlDoc.NewElement("config");
	xmlDoc.InsertEndChild(config);
	
	for (int sheet_index = 1; sheet_index < m_xls->sheetCount(); ++sheet_index)
	{
		this->SheetToXmlEle(&xmlDoc, sheet_index);
	}
	
	if (xmlDoc.SaveFile(m_xml_file_path.c_str()) != tinyxml2::XML_SUCCESS)
	{
		return false;
	}

	m_xls->release();
	return true;
}

void XlsConverXml::GetXmlPathByXlsPath(const string &xls_file_path, string *xml_path)
{
	if (xls_file_path.empty())
	{
		printf("%s, %d\n", __FUNCTION__, __LINE__);
		return;
	}

	this->FormatFilePath(const_cast<string&>(xls_file_path), xml_path);
	
}

void XlsConverXml::FormatFilePath(string &file_path, string *format_path)
{
	if (m_xls_file_path.empty())
	{
		printf("%s, %d\n", __FUNCTION__, __LINE__);
		return;
	}
	
	string temp_path = file_path;
	//string::size_type first = temp_path.find_first_not_of("\\");
	//string::size_type last = temp_path.find_last_not_of("\\");
	//if (first == string::npos || last == string::npos)
	//{
	//	printf("%s, %d\n", __FUNCTION__, __LINE__);
	//	return;
	//}

	//while (first != string::npos)
	//{
	//	string::size_type pos = first + 1;
	//	temp_path.insert(pos, "\\");
	//	first = temp_path.find_first_not_of("\\", pos + 1);
	//}

	string xml = ".xml";
	string::size_type pos = temp_path.find_last_of('.');
	temp_path.replace(pos, 4, xml);
	*format_path = temp_path;
}

bool XlsConverXml::CreatBookAndLoadXls()
{
	char exten_name[MAX_PATH] = { 0 };
	this->GetFileExtenName(m_xls_file_path, exten_name);
	if (strcmp(exten_name, ".xls") == 0)
	{
		m_xls = xlCreateBook();
	}
	else if (strcmp(exten_name, ".xlsx") == 0)
	{
		m_xls = xlCreateXMLBook();
	}
	else
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "UnkownFileType");
		return false;
	}

	if (!m_xls->load(m_xls_file_path.c_str()))
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "LoadFileError");
		return false;
	}

	return true;
}

void XlsConverXml::InitEachSheetNameAndRout()
{
	// ��ȡ���ñ������
	if (NULL == m_xls)
	{
		return;
	}

	SheetDefineInfo sheet_info;
	
	// ����±꣬�ᣬ�ݴ�0��ʼ�� firstRow, firstCol�Ǵ����õ������п�ʼ
	int first_row = m_xls->getSheet(0)->firstRow();
	int first_col = m_xls->getSheet(0)->firstCol();

	m_xml_file_name = this->ReadCellContent(m_xls->getSheet(0), first_row, first_col);
	m_row_begin_read = atoi(this->ReadCellContent(m_xls->getSheet(0), first_row, first_col + 1));

	for (int sheet_index = 1; sheet_index < m_xls->sheetCount(); ++sheet_index)
	{
		// ����������ͱ�����
		SheetDefineInfo sheet_info;
		sheet_info.sheet_index = sheet_index;
		sheet_info.sheet_name = this->ReadCellContent(m_xls->getSheet(0), first_row, 2);		//���ñ��һ��Ŀ¼��,�ӵ�����������Ϊ�����ֶ�

		// ����ÿ������ֶκ�����
		int first_row = m_xls->getSheet(sheet_index)->firstRow();
		int read_row = first_row + 2;
		for (int col_index = 0; NULL != ReadCellContent(m_xls->getSheet(sheet_index), read_row, col_index); ++read_row,++col_index)
		{
			ColAttr col_attr;
			std::string str_col_type = this->ReadCellContent(m_xls->getSheet(sheet_index), first_row, col_index);
			col_attr.col_type = this->GetColType(str_col_type);
			col_attr.col_name = this->ReadCellContent(m_xls->getSheet(sheet_index), first_row + 2, col_index);

			sheet_info.col_info_list.insert(std::pair<int, ColAttr>(col_index, col_attr));
		}

		m_sheet_info_list.insert(std::pair<int, SheetDefineInfo>(sheet_index, sheet_info));
	}


}

//void XlsConverXml::InitEachSheetColName()
//{
//	if (NULL == m_xls)
//	{
//		return;
//	}
//
//	for (int sheet_index = 1; sheet_index < m_xls->sheetCount(); ++sheet_index)
//	{
//		int first_row = m_xls->getSheet(sheet_index)->firstRow();
//		for (int col_index = 0; NULL != ReadCellContent(m_xls->getSheet(sheet_index), first_row + 2, col_index); ++col_index)
//		{
//			map<int, const char*> &sheet_col_map = m_col_name_map[sheet_index];
//			const char* text = this->ReadCellContent(m_xls->getSheet(sheet_index), first_row + 2, col_index);
//			sheet_col_map[col_index] = text;
//		}
//	}
//}

//void XlsConverXml::InitEachSheetColType()
//{
//	if (NULL == m_xls)
//	{
//		return;
//	}
//
//	for (int sheet_index = 1; sheet_index < m_xls->sheetCount(); ++sheet_index)
//	{
//		int first_row = m_xls->getSheet(sheet_index)->firstRow();
//		for (int col_index = 0; NULL != ReadCellContent(m_xls->getSheet(sheet_index), first_row, col_index); ++col_index)
//		{
//			const char* text = this->ReadCellContent(m_xls->getSheet(sheet_index), first_row, col_index);
//			string str_text(text);
//			int col_type = this->GetColType(str_text);
//			map<int, int> &sheet_col_type_map = m_col_type_map[sheet_index];
//			sheet_col_type_map[col_index] = col_type;
//		}
//	}
//}

void XlsConverXml::GetFileExtenName(const string& file_path, char exten_name[])
{
	if (file_path.empty())
	{
		printf("%s, %d\n", __FUNCTION__, __LINE__);
		return;
	}

	string::size_type last = file_path.find_last_of('.');
	string temp_name = file_path.substr(last);
	int strsize = temp_name.size();
	memcpy(exten_name, temp_name.c_str(), strsize);
	exten_name[strsize++] = '\0';
}

int XlsConverXml::SheetToXmlEle(tinyxml2::XMLDocument *xmlDoc, int sheet_index)
{
	if (sheet_index <= 0 || sheet_index > m_xls->sheetCount())
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "SheetNoExist");
		return -1;
	}

	if (NULL == xmlDoc)
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "xmlDocNULL");
		return -2;
	}

	const char* Element_name = Element_name = m_sheet_info_list[sheet_index].sheet_name.c_str();
	if (NULL == Element_name)
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "Element_nameError");
	}

	tinyxml2::XMLElement* root = xmlDoc->RootElement();
	tinyxml2::XMLElement *sheet_element = xmlDoc->NewElement(Element_name);
	if (NULL == root || NULL == sheet_element)
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "xmlDocNULL");
		return -3;
	}

	// ����ÿ����Ԫ��
	libxl::Sheet *sheet = m_xls->getSheet(sheet_index);
	if (NULL == sheet)
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "SheetNULL");
		return -4;
	}

	int first_row = m_xls->getSheet(sheet_index)->firstRow();
	for (int row_index = first_row + 3; row_index < sheet->lastRow(); ++row_index)
	{
		tinyxml2::XMLElement* datanode = xmlDoc->NewElement("data");
		sheet_element->InsertEndChild(datanode);

		for (int col_index = 0; col_index < sheet->lastCol(); ++col_index)
		{
			// ���ļ�д��Ԫ������
			int col_type = m_sheet_info_list[sheet_index].col_info_list[col_index].col_type;
			this->WriteCellTextToXml(xmlDoc, datanode, sheet_index, row_index, col_index, col_type);
		}
	}

	root->InsertEndChild(sheet_element);
	printf("%s\n", "FinishConver");
	return 0;
}

int XlsConverXml::GetColType(const string &str)
{
	if (str.empty())
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "StringEmpty");
		return -1000;
	}

	const char* temp_str = str.c_str();
	if (strcmp(temp_str, "cs") == 0)
	{
		return COLTYPE_CS;
	}
	else if (strcmp(temp_str, "s") == 0)
	{
		return COLTYPE_S;
	}
	else if (strcmp(temp_str, "c") == 0)
	{
		return COLTYPE_C;
	}
	else if (strcmp(temp_str, "cs,item") == 0)
	{
		return COLTYPE_CS_ITEM;
	}
	else if (strcmp(temp_str, "cs,itemlist") == 0)
	{
		return COLTYPE_CS_ITEM_LIST;
	}
	else
	{
		return -10000;
	}
}

const char* XlsConverXml::GetColNameBySheetAndCol(int sheet_index, int col_index)
{
	// �ж�
	return m_sheet_info_list[sheet_index].col_info_list[col_index].col_name.c_str();
}

const char* XlsConverXml::ReadCellContent(libxl::Sheet *sheet, const int32_t row, const int32_t col)
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
				this->GBKToUTF8(text, wcharTemp, textUtf8Str, textUtf8StrLen);
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

void XlsConverXml::WriteCellTextToXml(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement* include_on, int sheet, int row, int col, int col_type)
{


	// ����Ԫ�����ƺ�ֵ
	const char* cell_name = this->GetColNameBySheetAndCol(sheet, col);
	const char* cell_value = this->ReadCellContent(m_xls->getSheet(sheet), row, col);

	switch (col_type)
	{
	case COLTYPE_C:
	{

	}
	break;

	case COLTYPE_S:
	{
		tinyxml2::XMLElement* namenode = xmlDoc->NewElement(cell_name);
		tinyxml2::XMLText* cellvalue = xmlDoc->NewText(cell_value);
		namenode->InsertEndChild(cellvalue);
		include_on->InsertEndChild(namenode);
	}
	break;

	case COLTYPE_CS:
	{

	}
	break;

	case COLTYPE_CS_ITEM:
	{

	}
	break;

	case COLTYPE_CS_ITEM_LIST:
	{

	}
	break;

	default:
	{

	}
	break;

	}
}

void XlsConverXml::GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen)
{
	int32_t len = MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, NULL, 0); // ת�����ֽ��ַ�����unicode�ַ��������ַ�����
	MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, wcharTmp, len);		 // ת��
	len = WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, NULL, 0, NULL, NULL); // unicode�ַ���ת�����ֽ��ַ��������ַ�����
	WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, utf8Buf, len, NULL, NULL);  // ת��
}
