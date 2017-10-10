#include "Hash.h"

// ������
Hash::Hash(void) {
	// ��ʼ��CRC��
	for (unsigned int i = 0; i < sizeof(m_crcTable) /
		sizeof(m_crcTable[0]); ++i) {
		unsigned int crc = i;

		for (unsigned j = 0; j < 8; ++j)
			if (crc & 1)
				crc = crc >> 1 ^ 0xEDB88320;
			else
				crc = crc >> 1;

		m_crcTable[i] = crc;
	}
}

// Times33��ϣ�㷨
// ���ز����ַ�����32λ��ϣֵ
// hash(i) = hash(i-1)��33+str[i] (hash(-1) = 0)
unsigned int Hash::RecalHashByTimes33(
	string const& str // [����] ����ϣ�ַ���
	) const {
	unsigned int val = 0;

	size_t len = str.size();
	for (size_t i = 0; i < len; ++i)
		val = (val << 5) + val + (unsigned char)str[i];

	return val;
}

// TimesN��ϣ�㷨
// ���ز����ַ�����32λ��ϣֵ
// hash(i) = hash(i-1)��N+str[i] (hash(-1) = 0)
unsigned int Hash::timesnum(
	string const& str, // [����] ����ϣ�ַ���
	int           num  // [����] N
	) const {
	unsigned int val = 0;

	size_t len = str.size();
	for (size_t i = 0; i < len; ++i)
		val = val * num + (unsigned char)str[i];

	return val;
}

// AP��ϣ�㷨
// ���ز����ַ�����32λ��ϣֵ
unsigned int Hash::aphash(
	string const& str // [����] ����ϣ�ַ���
	) const {
	unsigned int val = 0;

	size_t len = str.size();
	for (size_t i = 0; i < len; ++i)
		if (i & 1)
			val ^= ~(val << 11 ^ (unsigned char)str[i] ^ val >> 5);
		else
			val ^= val << 7 ^ (unsigned char)str[i] ^ val >> 3;

	return val & 0x7FFFFFFF;
}

// FNV��ϣ�㷨
// ���ز����ַ�����32λ��ϣֵ
unsigned int Hash::hash16777619(
	string const& str // [����] ����ϣ�ַ���
	) const {
	unsigned int val = 0;

	size_t len = str.size();
	for (size_t i = 0; i < len; ++i) {
		val *= 16777619;
		val ^= (unsigned char)str[i];
	}

	return val;
}

// MySQL��ϣ�㷨
// ���ز����ַ�����32λ��ϣֵ
unsigned int Hash::mysqlhash(
	string const& str // [����] ����ϣ�ַ���
	) const {
	unsigned int nr1 = 1, nr2 = 4;

	size_t len = str.size();
	for (size_t i = 0; i < len; ++i) {
		nr1 ^= ((nr1 & 63) + nr2) * (unsigned char)str[i] + (nr1 << 8);
		nr2 += 3;
	}

	return nr1;
}

// ѭ������У���㷨
// ���ز����ַ�����ѭ������У����
unsigned int Hash::crc32(
	string const& str // [����] ��У���ַ���
	) const {
	unsigned int val = 0xFFFFFFFF;

	size_t len = str.size();
	for (size_t i = 0; i < len; ++i)
		val = m_crcTable[(val & 0xFF) ^ (unsigned char)str[i]] ^ val >> 8;

	return ~val;
}


