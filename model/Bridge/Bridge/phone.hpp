#ifndef __PHONE_HPP_
#define __PHONE_HPP_

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "softwares.hpp"

class Phone
{
public:
	Phone();
	virtual ~Phone();

	// 抽象接口，提供扩充功能（修改抽象）
	bool InstallSoftWares(SoftWares* softwares, const int softwares_name);
	virtual void StartSoftWares(int softwares_name) {}

	SoftWares* m_softwares_list[5];
};

Phone::Phone()
{
	for (int i = 0; i < 5; ++i)
	{
		m_softwares_list[i] = NULL;
	}
}

Phone::~Phone()
{
	for (int i = 0; i < 5; ++i)
	{
		if (NULL != m_softwares_list[i])
		{
			delete m_softwares_list[i];
		}

		memset(m_softwares_list, 0, sizeof(m_softwares_list));
	}
}

bool Phone::InstallSoftWares(SoftWares* softwares, const int softwares_name)
{
	if (softwares_name < 0 || softwares_name >= 5)
	{
		return false;
	}

	m_softwares_list[softwares_name] = softwares;
	return true;
}

#endif
