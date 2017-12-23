#ifndef __FACTORY_HPP_
#define __FACTORY_HPP_

#include "product.hpp"

class Factory{
public:
	Factory(){}
	~Factory(){}

	virtual Product* CreatProduct(int product_type) { return NULL; }
};
#endif // !__FACTORY_HPP_
