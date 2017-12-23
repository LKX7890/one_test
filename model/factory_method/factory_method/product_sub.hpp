#ifndef __PRODUCT_SUB_HPP_
#define __PRODUCT_SUB_HPP_

#include "product.hpp"

class ProductSub : public Product
{
public:
	ProductSub(): Product() {}
	~ProductSub() {}

	virtual int Operation() { return a - b; }
};
#endif