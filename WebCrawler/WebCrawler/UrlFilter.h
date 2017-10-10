#ifndef __URLFILTER_H__
#define __URLFILTER_H__

class UrlFilter
{
public:

	virtual ~UrlFilter(void) {}
	virtual bool IsUrlExist(string const &strUrl) = 0;
};
#endif
