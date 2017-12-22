#ifndef __FACTORY_HPP_
#define __FACTORY_HPP_
#include "chinesebrush.hpp"
#include "ChineseSword.hpp"
class Product;

enum PRODUCT_TYPE
{
	PRODUCT_TYPE_BRUSH,
	PRODUCT_TYPE_SWORD,
};

class Factory
{
public:
	Factory(){}
	~Factory(){}

	static Product* CreateProduct(const int product_type);
};

 Product* Factory::CreateProduct(const int product_type)
{
	switch (product_type)
	{
	case PRODUCT_TYPE_BRUSH:
	{
		return new ChineseBrush;
	}
	break;

	case PRODUCT_TYPE_SWORD:
	{
		return new ChineseSword;
	}
	break;

	default:
	{
		return NULL;
	}
	break;
	}

	return NULL;
}
#endif