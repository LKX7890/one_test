#ifndef __BLOOMFILTER_H
#define __BLOOMFILTER_H

#include "Hash.h"
#include "UrlFilter.h"

class BloomFilter : public UrlFilter
{
public:

	BloomFilter(void);

	bool IsUrlExist(string const &strUrl);
private:

	static int const HASH_FUNCS = 8;
	static size_t const BLOOMTABLE_SIZE = 1000000;

	unsigned int RecalHash(int id, string const &strUrl) const;

	Hash m_hash;
	unsigned int m_bloomTable[BLOOMTABLE_SIZE];
};
#endif