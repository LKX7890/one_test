#include <cstdlib>
#include <cstdio>

#include "xlsconverxml/xlsconverxml.h"

int main()
{
	string xls_path = "G:\\one_test\\xlsconverxml\\file\\test.xls";
	XlsConverXml xlstoxml(xls_path);
	xlstoxml.StartXlsConverXml();
	getchar();
	return 0;
}