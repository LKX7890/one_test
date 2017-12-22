#include <cstdlib>
#include <cstdio>

#include "xlsconverxml.h"

int main()
{
	std::string xls_path = ".\\..\\file\\test.xls";
	XlsConverXml xlstoxml(xls_path);
	xlstoxml.StartXlsConverXml();
	getchar();
	return 0;
}