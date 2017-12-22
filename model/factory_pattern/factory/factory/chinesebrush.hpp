#ifndef __CHINESEBRUSH_HPP_
#define __CHINESEBRUSH_HPP_

#include "Product.hpp"

class ChineseBrush : public Product
{
public:
	ChineseBrush(){}
	~ChineseBrush(){}

	void Operation(){ printf("lian shu fa.\n"); }
};
#endif
