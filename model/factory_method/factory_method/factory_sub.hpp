#ifndef __FACTORY_SUB_HPP_
#define __FACTORY_SUB_HPP_

#include "product.hpp"
#include "factory.hpp"
#include "product_sub.hpp"

class FactorySub : public Factory
{
public:
	FactorySub() {}
	~FactorySub() {}

	virtual Product* CreatProduct() { return new ProductSub; }
};
#endif