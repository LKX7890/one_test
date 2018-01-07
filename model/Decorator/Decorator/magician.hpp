#ifndef __MAGICIAN_HPP_
#define __MAGICIAN_HPP_

#include "Profession.hpp"

class Magician : public Profession
{
public:
	Magician() {}
	~Magician(){}

	virtual void PerfMagicSkill() { std::cout << "地狱火三秒后释放" << std::endl; }
	virtual void PerfNormalSkill() { std::cout << "连击后紧接着" << std::endl; }
};
#endif