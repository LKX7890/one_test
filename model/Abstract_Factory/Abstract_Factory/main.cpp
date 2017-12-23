#include <iostream>
#include "factoryA.hPP"
#include "factoryB.hpp"

int main()
{
	FactoryA factory_a;
	FactoryB factory_b;

	factory_a.CreatProduct(FactoryA::FACTORY_A_TYPE_A);
	factory_a.CreatProduct(FactoryA::FACTORY_A_TYPE_VIP);
	factory_b.CreatProduct(FactoryB::FACTORY_B_TYPE_B);
	factory_b.CreatProduct(FactoryB::FACTORY_B_TYPE_VIP);
	getchar();
	return 0;
}