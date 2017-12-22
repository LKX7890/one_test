#include "xlsconverxml.h"


XlsConverXml::XlsConverXml(const string &xls_file_path) : m_xls(NULL)
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

	// ����xls��book
	bool ret = this->CreatBookAndLoadXls();
	if (!ret) return false;

	// ��ȡxls���ö�����Ϣ
	bool succ = this->ReadSheetDefineInfo();
	if (!succ) return false;

	// ����xml�ļ�
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLDeclaration *declaration = xmlDoc.NewDeclaration("version=\"1.0\" encoding=\"utf-8\"");
	xmlDoc.LinkEndChild(declaration);
	tinyxml2::XMLElement *config = xmlDoc.NewElement("config");
	xmlDoc.InsertEndChild(config);
	
	// ÿ����ת����xmlԪ��
	for (int sheet_index = 1; sheet_index < m_xls->sheetCount(); ++sheet_index)
	{
		this->SheetConverXmlFile(&xmlDoc, sheet_index);
	}
	
	// ����
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

void XlsConverXml::FormatFilePath(const string &file_path, string *format_path)
{
	if (m_xls_file_path.empty())
	{
		printf("%s, %d\n", __FUNCTION__, __LINE__);
		return;
	}
	
	string temp_path = file_path;

	string xml = ".xml";
	string::size_type pos = temp_path.find_last_of('.');
	temp_path.replace(pos, 4, xml);
	*format_path = temp_path;
}

bool XlsConverXml::CreatBookAndLoadXls()
{
	char exten_name[MAX_PATH] = { 0 };
	this->GetFileTypeName(m_xls_file_path, exten_name);
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

bool XlsConverXml::ReadSheetDefineInfo()
{
	// ��ȡ���ñ������
	if (NULL == m_xls)
	{
		return false;
	}

	SheetDefineInfo sheet_info;
	
	// ����±꣬�ᣬ�ݴ�0��ʼ�� firstRow, firstCol�Ǵ����õ������п�ʼ
	int first_row = m_xls->getSheet(0)->firstRow();
	int first_col = m_xls->getSheet(0)->firstCol();

	int sheet_index_begin = 1;
	int col_index_begin = 2;
	int row_index_valid = 2;
	for (int sheet_index = sheet_index_begin, define_sheet_col = col_index_begin; sheet_index < m_xls->sheetCount(); ++sheet_index, ++define_sheet_col)
	{
		// ����������ͱ�����
		SheetDefineInfo sheet_info;
		sheet_info.sheet_index = sheet_index;
		sheet_info.sheet_name = this->ReadCellContent(m_xls->getSheet(0), first_row, define_sheet_col);		//���ñ��һ��Ŀ¼��,�ӵ�����������Ϊ�����ֶ�

		// ����ÿ������е��ֶκ�����
		int first_row = m_xls->getSheet(sheet_index)->firstRow();
		int read_row = first_row + row_index_valid;
		for (int cur_sheet_col = 0; NULL != ReadCellContent(m_xls->getSheet(sheet_index), read_row, cur_sheet_col); ++read_row,++cur_sheet_col)
		{
			ColAttr col_attr;
			std::string str_col_type = this->ReadCellContent(m_xls->getSheet(sheet_index), first_row, cur_sheet_col);
			col_attr.col_type = this->GetColType(str_col_type);
			col_attr.col_name = this->ReadCellContent(m_xls->getSheet(sheet_index), first_row + row_index_valid, cur_sheet_col);

			sheet_info.col_info_list.insert(std::pair<int, ColAttr>(cur_sheet_col, col_attr));
		}

		m_sheet_info_list.insert(std::pair<int, SheetDefineInfo>(sheet_index, sheet_info));
	}


}

void XlsConverXml::GetFileTypeName(const string& file_path, char exten_name[])
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

int XlsConverXml::SheetConverXmlFile(tinyxml2::XMLDocument *xmlDoc, const int sheet_index)
{
	if (NULL == xmlDoc)
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "xmlDocNULL");
		return -1;
	}

	if (sheet_index <= 0 || sheet_index > m_xls->sheetCount())
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "SheetNoExist");
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
			this->WriteCellTextToXmlFile(xmlDoc, datanode, sheet_index, row_index, col_index, col_type);
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

std::string XlsConverXml::GetColNameBySheetAndCol(const int sheet_index, const int col_index)
{
	// �ж�
	return m_sheet_info_list[sheet_index].col_info_list[col_index].col_name;
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
			static char doubleStr[doubleStrLen] = { 0 };
			sprintf_s(doubleStr, doubleStrLen, "%g", sheet->readNum(row, col));
			for (int32_t i = strlen(doubleStr) - 1; i >= 0; --i)		//�����ݺ��濪ʼʶ��
			{
				if (doubleStr[i] >= '0' && doubleStr[i] <= '9')
				{
					break;
				}
			}
			return doubleStr;
		} while (0);

	default:
		break;
	}
	return NULL;
}

void XlsConverXml::WriteCellTextToXmlFile(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement* include_on, int sheet, int row, int col, int col_type)
{


	// ����Ԫ�����ƺ�ֵ
	std::string cell_name = this->GetColNameBySheetAndCol(sheet, col);
	std::string cell_value = this->ReadCellContent(m_xls->getSheet(sheet), row, col);

	switch (col_type)
	{
	case COLTYPE_S:
	case COLTYPE_CS:
	{
		tinyxml2::XMLElement* namenode = xmlDoc->NewElement(cell_name.c_str());
		tinyxml2::XMLText* cellvalue = xmlDoc->NewText(cell_value.c_str());
		namenode->InsertEndChild(cellvalue);
		include_on->InsertEndChild(namenode);
	}
	break;

	case COLTYPE_CS_ITEM:
	{
		this->OnXmlWriteItemInfoList(xmlDoc, include_on, cell_name, COLTYPE_CS_ITEM, cell_value);
	}
	break;

	case COLTYPE_CS_ITEM_LIST:
	{
		this->OnXmlWriteItemInfoList(xmlDoc, include_on, cell_name, COLTYPE_CS_ITEM_LIST, cell_value);
	}
	break;

	default:
	{
		printf("Unknown col_type !!!\n");
	}
	break;

	}
}

void XlsConverXml::GetItemInfoFromCell(int cell_type, std::string cell_val, std::vector<ItemInfo>& item_list)
{
	if (cell_type < COLTYPE_CS || cell_type > COLTYPE_CS_ITEM_LIST)
	{
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, "GetItemInfoFromCell");
		return;
	}

	// ��ȡ��Ʒ�б���
	std::string delim(";");
	std::vector<std::string> item;
	if (COLTYPE_CS_ITEM == cell_type || COLTYPE_CS_ITEM_LIST == cell_type)
	{
		this->StrSplit(cell_val, delim, item);
	}

	// ��ȡ��Ʒ��Ϣ
	std::string item_delim(":");
	std::vector<std::string> item_attr;
	for (size_t i = 0; i < item.size(); ++i)
	{
		this->StrSplit(item[i], item_delim, item_attr);

		ItemInfo temp;

		temp.item_id = item_attr[0];
		temp.item_num = item_attr[1];
		temp.is_bind = item_attr[2];

		item_list.push_back(temp);
		item_attr.clear();
	}
}

void XlsConverXml::GBKToUTF8(const char* asciiBuf, WCHAR wcharTmp[], char utf8Buf[], const int32_t utf8BufLen)
{
	int32_t len = MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, NULL, 0); // ת�����ֽ��ַ�����unicode�ַ��������ַ�����
	MultiByteToWideChar(CP_ACP, 0, asciiBuf, -1, wcharTmp, len);		 // ת��
	len = WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, NULL, 0, NULL, NULL); // unicode�ַ���ת�����ֽ��ַ��������ַ�����
	WideCharToMultiByte(CP_UTF8, 0, wcharTmp, -1, utf8Buf, len, NULL, NULL);  // ת��
}

void XlsConverXml::StrSplit(std::string& str, std::string& delim, std::vector<std::string>& ret)
{
	size_t last = 0;
	size_t index = str.find_first_of(delim, last);

	while (index != string::npos)
	{
		ret.push_back(str.substr(last, index - last));
		last = index + 1;
		index = str.find_first_of(delim, last);
	}

	if ((index - last) > 0)
	{
		ret.push_back(str.substr(last, index - last));
	}
}

void XlsConverXml::OnXmlWriteItemInfoList(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLElement* include_on, const std::string cell_name, const int cell_type, const std::string cell_val)
{
	std::vector<ItemInfo> item_list;
	this->GetItemInfoFromCell(cell_type, cell_val, item_list);

	tinyxml2::XMLElement* include = NULL;
	if (COLTYPE_CS_ITEM_LIST == cell_type)
	{
		std::string list_name = cell_name + "_list";
		tinyxml2::XMLElement* listname = xmlDoc->NewElement(list_name.c_str());		//�����ֶ�
		include_on->InsertEndChild(listname);

		include = listname;
	}
	else
	{
		include = include_on;
	}
	
	for (size_t i = 0; i < item_list.size(); ++i)
	{
		tinyxml2::XMLElement* nodename = xmlDoc->NewElement(cell_name.c_str());		//�����ֶ�
		include->InsertEndChild(nodename);

		tinyxml2::XMLElement* node1 = xmlDoc->NewElement("item_id");						// �����ӽڵ㲢����
		nodename->LinkEndChild(node1);													// �����ӽڵ������
		tinyxml2::XMLText* cellvalue1 = xmlDoc->NewText(item_list[i].item_id.c_str());	// �����ӽڵ��ֵ
		node1->LinkEndChild(cellvalue1);													// �����ӽڵ�ֵ


		tinyxml2::XMLElement* node2 = xmlDoc->NewElement("item_num");
		nodename->LinkEndChild(node2);
		tinyxml2::XMLText* cellvalue2 = xmlDoc->NewText(item_list[i].item_num.c_str());
		node2->LinkEndChild(cellvalue2);

		tinyxml2::XMLElement* node3 = xmlDoc->NewElement("is_bind");
		nodename->LinkEndChild(node3);
		tinyxml2::XMLText* cellvalue3 = xmlDoc->NewText(item_list[i].is_bind.c_str());
		node3->LinkEndChild(cellvalue3);

	}
}
