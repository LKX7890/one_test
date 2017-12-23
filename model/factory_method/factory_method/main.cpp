#include <iostream>
#include "factory.hpp"
#include "factory_add.hpp"
#include "factory_sub.hpp"

int main()
{
	std::cout << "please enter:0 or 1" << std::endl;
	int type;
	std::cin >> type;

	Factory *factory = NULL;
	switch (type)
	{
		case Factory::FACTORY_TYPE_ADD:
		{
			factory = new FactoryADD;
		}
		break;

		case Factory::FACTORY_TYPE_SUB:
		{
			factory = new FactorySub;
		}
		break;

		default:
		{
			factory = NULL;
			std::cout << "Unknown type" << std::endl;
			return 0;
		}
		break;
	}

	Product *product = factory->CreatProduct();
	product->a = 8;
	product->b = 3;

	int oper_result = 0;
	oper_result = product->Operation();

	std::cout << oper_result << std::endl;

	system("pause");
	return 0;
}