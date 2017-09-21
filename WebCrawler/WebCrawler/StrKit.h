#ifndef __STRKIT_H__
#define __STRKIT_H__

#include "common.h"

class StrKit
{
public:

	static string StrCat(char const* str1, char const* str2, ...);
	static string& StrTrim(string& str);
	static vector<string>StrSplit(string const& str, string const& delim, int limit = 0);
};
#endif