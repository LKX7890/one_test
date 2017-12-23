#ifndef __FACTORY_B_HPP_
#define __FACTORY_B_HPP_

#include "factory.h"
#include "product.hpp"
#include "productB.hPP"
#include "productBvip.hpp"

class FactoryB : public Factory
{
public:
	enum FACTORY_B_TYPE
	{
		FACTORY_B_TYPE_B,
		FACTORY_B_TYPE_VIP,
	};

	FactoryB(){}
	~FactoryB(){}

	virtual Product* CreatProduct(int product_type);
};

Product* FactoryB::CreatProduct(int product_type)
{
	switch (product_type)
	{
	case FactoryB::FACTORY_B_TYPE_B:
	{
		ProductB product_b;
		return &product_b;
	}
	break;

	case FactoryB::FACTORY_B_TYPE_VIP:
	{
		ProductBVip productb_vip;
		return &productb_vip;
	}
	break;

	default:
		return NULL;
		break;
	}
}
#endif // !__FACTORY_B_HPP_
