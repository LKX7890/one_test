#include <cstdio>
#include "Factory.hpp"

int main()
{
	Product *pbrush = Factory::CreateProduct(PRODUCT_TYPE_BRUSH);
	pbrush->Operation();
	getchar();
	delete pbrush;
	pbrush = NULL;
	return 0;
}