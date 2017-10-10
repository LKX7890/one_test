#ifndef __HASH_H__
#define __HASH_H__

#include "common.h"

class Hash
{
public:

	Hash(void);
	unsigned int RecalHashByTimes33(string const &str) const;
	unsigned int timesnum(string const &str, int num) const;
	unsigned int aphash(string const &str) const;
	unsigned int hash16777619(string const &str) const;
	unsigned int mysqlhash(string const &str) const;
	unsigned int crc32(string const &str) const;

private:

	unsigned int m_crcTable[256];
};
#endif