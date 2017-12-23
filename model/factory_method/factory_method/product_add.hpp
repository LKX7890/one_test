#ifndef __PRODUCT_ADD_HPP_
#define __PRODUCT_ADD_HPP_

#include "product.hpp"

class ProductAdd : public Product
{
public:
	ProductAdd() : Product() {}
	~ProductAdd() {}

	virtual int Operation() { return a + b; }
};
#endif