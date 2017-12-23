#ifndef __PRODUCT_HPP_
#define __PRODUCT_HPP_

class Product
{
public:
	Product() : a(0), b(0){}
	~Product() {}

	virtual int Operation() { return NULL; }
	int a;
	int b;
};
#endif