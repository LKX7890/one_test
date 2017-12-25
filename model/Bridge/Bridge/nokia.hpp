#ifndef __NOKIA_HPP_
#define __NOKIA_HPP_

#include "phone.hpp"
#include "softwares.hpp"

class Nokia : public Phone
{
public:
	Nokia() :Phone() {}
	~Nokia(){}

	virtual void StartSoftWares(int softwares_name);
};

void Nokia::StartSoftWares(int softwares_name)
{
	if (softwares_name < 0 || softwares_name >= 5)
	{
		return;
	}

	// 基类指针数组被赋值子类指针时，数组元素对成员的可见降低到基类可见的范围，
	// 但是可以通过可见范围的函数调用不可见函数，调用成员函数是默认传入this指针
	m_softwares_list[softwares_name]->OnStartSoftWares();
}

#endif
