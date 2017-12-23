#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_

#include <cstdlib>
#include <iostream>

class Singleton
{
public:
	static Singleton* Install();

private:
	Singleton(){}
	~Singleton(){}

	static Singleton* m_singleton;
};

Singleton* Singleton::m_singleton = NULL;

Singleton* Singleton::Install()
{
	if (NULL != m_singleton)
	{
		std::cout << "return old" << std::endl;
		return m_singleton;
	}
	else
	{
		std::cout << "new obj" << std::endl;
		m_singleton = new Singleton;
		return m_singleton;
	}
}
#endif