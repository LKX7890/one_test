#ifndef __FACTORY_HPP_
#define __FACTORY_HPP_

#include "product.hpp"

class Factory
{
public:
	enum FACTORY_TYPE
	{
		FACTORY_TYPE_ADD,
		FACTORY_TYPE_SUB,
	};

	Factory() {}
	~Factory() {}

	virtual Product* CreatProduct() { return NULL; }
};
#endif