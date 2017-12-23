#ifndef __FACTORY_A_HPP_
#define __FACTORY_A_HPP_

#include "factory.h"
#include "product.hpp"
#include "productA.hpp"
#include "ProductAVip.hpp"

class FactoryA : public Factory
{
public:
	enum FACTORY_A_TYPE
	{
		FACTORY_A_TYPE_A,
		FACTORY_A_TYPE_VIP,
	};

	FactoryA(){}
	~FactoryA(){}

	static FactoryA* GetInstall();
	virtual Product* CreatProduct(int product_type);
private:
	static FactoryA * m_factoryA;
};

FactoryA* FactoryA::m_factoryA = NULL;

Product* FactoryA::CreatProduct(int product_type)
{
	switch (product_type)
	{
		case FACTORY_A_TYPE_A:
		{
			ProductA product_a;
			return &product_a;
		}
		break;

		case FACTORY_A_TYPE_VIP:
		{
			ProductAVip producta_vip;
			return &producta_vip;
		}
		break;

		default:
			return NULL;
			break;
	}
}

FactoryA* FactoryA::GetInstall()
{
	if (NULL != m_factoryA)
	{
		return m_factoryA;
	}
	else
	{
		m_factoryA = new FactoryA;
		return m_factoryA;
	}
}
#endif // !__FACTORY_A_HPP_
