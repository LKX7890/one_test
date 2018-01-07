#ifndef _MAGIC_SKILL_HPP_
#define _MAGIC_SKILL_HPP_

#include "Skill.hpp"

class MagicSkill : public Skill
{
public:
	MagicSkill() {}
	~MagicSkill() {}

	// 维护原有功能同时添加新功能
	virtual void PerfMagicSkill() { m_profession->PerfMagicSkill(); std::cout << "末日火雨" << std::endl; }
};
#endif