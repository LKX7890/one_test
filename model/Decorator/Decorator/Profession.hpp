#ifndef __PROFESSION_HPP_
#define __PROFESSION_HPP_

#include <cstdlib>
#include <iostream>

class Profession
{
public:
	Profession(){}
	~Profession(){}

	virtual void PerfMagicSkill() = 0;
	virtual void PerfNormalSkill() = 0;
};
#endif