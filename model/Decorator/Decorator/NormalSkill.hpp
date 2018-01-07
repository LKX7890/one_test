#ifndef _NORMAL_SKILL_HPP_
#define _NORMAL_SKILL_HPP_

#include "Skill.hpp"

class NormalSkill : public Skill
{
public:
	NormalSkill() {}
	~NormalSkill() {}

	virtual void PerfNormalSkill() { m_profession->PerfNormalSkill(); std::cout << "挑空连击" << std::endl; }
};
#endif