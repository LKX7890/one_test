#include "StrKit.h"



std::string StrKit::StrCat(char const* str1, char const* str2, ...)
{
	string str = str1;
	str += str2;

	// 设置边长参数表
	va_list ap;
	va_start(ap, str2);

	// 将所有参数连接
	char const* strx = NULL;
	while (strx = va_arg(ap, char const*))
	{
		str += strx;
	}

	va_end(ap);

	return str;
}

std::string& StrKit::StrTrim(string& str)
{
	// 根据转义字符切出一段字符串
	string::size_type first = str.find_first_not_of(" \t\r\n");
	string::size_type last = str.find_last_not_of(" \t\r\n");

	if (first == string::npos || last == string::npos)
	{
		str.clear();
	}
	else
	{
		str = str.substr(first, last - first + 1);
	}

	return str;
}

std::vector<std::string> StrKit::StrSplit(string const& str, string const& delim, int limit /*= 0*/)
{
	vector<string>strv; // 保存拆分出的子字符串

	unsigned int length = str.size() + 1;
	char *temp = new char[length];
	strcpy_s(temp, length,str.c_str());

	// 根据传入分割符，切割子字符串，并放到容器中
	char *pNext = NULL;
	for (char* token = strtok_s(temp, delim.c_str(), &pNext); NULL != token; token = strtok_s(NULL, delim.c_str(), &pNext))
	{
		string part = token;
		strv.push_back(StrKit::StrTrim(part));

		if (!--limit && (token += strlen(token)) - temp < (int)str.size())
		{
			strv.push_back(StrKit::StrTrim(part = ++token));
			break;
		}
	}
	delete[] temp;
	return strv;
}

//int main(void)
//{
//	string str = StrKit::StrCat("123 = ", "456", " = 789", NULL);
//	cout << '[' << str << ']' << endl;
//
//	cout << '[' << StrKit::StrTrim(str) << ']' << endl;
//
//	vector<string> strv = StrKit::StrSplit(str, "=", 1); // 切割一次
//	for (vector<string>::const_iterator it = strv.begin(); it != strv.end(); ++it)
//	{
//		cout << '[' << *it << ']' << endl;
//	}
//
//	getchar();
//	return 0;
//}