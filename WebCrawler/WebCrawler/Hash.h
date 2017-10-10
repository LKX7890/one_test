#ifndef __HASH_H__
#define __HASH_H__

#include "common.h"

class Hash
{
public:

	Hash(void);
	unsigned int RecalHashByTimes33(string const &str) const;
	unsigned int RecalHashByTimesnum(string const &str, int num) const;
	unsigned int RecalHashByAphash(string const &str) const;
	unsigned int RecalHashByHash16777619(string const &str) const;
	unsigned int RecalHashByMysqlhash(string const &str) const;
	unsigned int RecalHashByCrc32(string const &str) const;

private:

	unsigned int m_crcTable[256];
};
#endif