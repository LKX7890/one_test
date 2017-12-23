#include "singleton.hpp"

int main()
{
	Singleton* singleton1 = Singleton::Install();
	Singleton* singleton2 = Singleton::Install();
	Singleton* singleton3 = Singleton::Install();

	getchar();
	return 0;
}