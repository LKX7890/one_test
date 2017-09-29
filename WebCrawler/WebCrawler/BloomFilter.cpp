#include "BloomFilter.h"



BloomFilter::BloomFilter(void)
{
	bzero(m_bllomTable, sizeof(m_bloomTable));
}

bool BloomFilter::exist(string const &strUrl)
{
	int one = 0;

	for (int i = 0; i < HASH_FUNCS; ++i)
	{
		//
		unsigned int bit = hash(i, strUrl) % (sizeof(m_bloomTable) * 8);
		unsigned int idx = bit / (sizeof(m_bloomTable[0]) * 8);
		bit %= sizeof(m_bloomTable[0]) * 8;

		if (m_bloomTable[idx] & 0X80000000 >> bit)
		{
			++one;
		}
		else
		{
			m_bloomTable[idx] |= 0X80000000 >> bit;
		}
	}

	return one == HASH_FUNCS;
}

unsigned int BloomFilter::hash(int id, string const &strUrl) const
{
	unsigned int val = 0;

	switch (id)
	{
	case 0:
		val = m_hash.times33(strUrl);
		break;

	case 1:
		val = m_hash.timesnum(strUrl, 31);
		break;

	case 2:
		val = m_hash.aphash(strUrl);
		break;

	case 3:
		val = m_hash.hash16777619(strUrl);
		break;
		
	case 4:
		val = m_hash.mysqlhash(strUrl);
		break;

	case 5:
		val = m_hash.crc32(strUrl);
		break;

	case 6:
		val = m_hash.timesnum(strUrl, 131);
		break;

	case 7:
		val = m_hash.timesnum(strUrl, 1313);
		break;

	default:
		g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "无效哈希算法标识：%d", id);
	}

	return val;
}
