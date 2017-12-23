#ifndef __PRODUCT_VIP_HPP_
#define __PRODUCT_VIP_HPP_

#include "product.hpp"

class ProductAVip : public Product
{
public:
	ProductAVip(){ std::cout << "creat productA_VIP" << std::endl; }
	~ProductAVip(){}

	virtual void Operation() {}
};
#endif 