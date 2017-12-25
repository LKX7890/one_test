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

	// ����ָ�����鱻��ֵ����ָ��ʱ������Ԫ�ضԳ�Ա�Ŀɼ����͵�����ɼ��ķ�Χ��
	// ���ǿ���ͨ���ɼ���Χ�ĺ������ò��ɼ����������ó�Ա������Ĭ�ϴ���thisָ��
	m_softwares_list[softwares_name]->OnStartSoftWares();
}

#endif
