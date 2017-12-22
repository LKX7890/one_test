#ifndef __CHINESESWORD_HPP_
#define __CHINESESWORD_HPP_

#include <cstdio>
#include "Product.hpp"

class ChineseSword : public Product
{
public:
	ChineseSword(){}
	~ChineseSword(){}

	virtual void Operation() { printf("lian wushu. \n"); }
};
#endif