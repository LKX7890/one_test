#ifndef __FACTORY_ADD_HPP_
#define __FACTORY_ADD_HPP_

#include "product.hpp"
#include "factory.hpp"
#include "product_add.hpp"

class FactoryADD : public Factory
{
public:
	FactoryADD() {}
	~FactoryADD() {}

	virtual Product* CreatProduct() { return new ProductAdd; }
};
#endif