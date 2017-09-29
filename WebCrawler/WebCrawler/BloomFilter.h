#ifndef __BLOOMFILTER_H
#define __BLOOMFILTER_H

class BloomFilter : public UrlFilter
{
public:

	BloomFilter(void);

	bool exist(string const &strUrl);
private:

	static int const HASH_FUNCS = 8;
	static size_t const BLOOMTABLE = 1000000;

	unsigned int hash(int id, string const &strUrl) const;

	Hash m_hash;
	unsigned int m_bloomTable[BLOOMTABLE_SIZE];
};
#endif