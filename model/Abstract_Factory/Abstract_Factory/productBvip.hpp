#ifndef __PRODUCT_B_VIP_HPP_
#define __PRODUCT_B_VIP_HPP_

#include "product.hpp"

class ProductBVip : public Product
{
public:
	ProductBVip(){ std::cout << "creat productB_VIP" << std::endl; }
	~ProductBVip(){}

	virtual void Operation() {}
};
#endif 