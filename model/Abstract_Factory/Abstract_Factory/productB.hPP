#ifndef __PRODUCT_B_HPP_
#define __PRODUCT_B_HPP_

#include "product.hpp"

class ProductB : public Product
{
public:
	ProductB(){ std::cout << "creat productB" << std::endl; }
	~ProductB(){}

	virtual void Operation() {}
};
#endif 