#ifndef __PRODUCT_A_HPP_
#define __PRODUCT_A_HPP_

#include "product.hpp"

class ProductA : public Product
{
public:
	ProductA(){ std::cout << "creat productA" << std::endl; }
	~ProductA(){}

	virtual void Operation() {}
};
#endif 