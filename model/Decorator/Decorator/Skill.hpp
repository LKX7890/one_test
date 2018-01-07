#ifndef __SKILL_HPP_
#define __SKILL_HPP_

#include "Profession.hpp"

// 修饰对象的基类，处在被修饰对象与修饰对象之间，减藕
class Skill : public Profession
{
public:
	Skill() : m_profession(NULL) {}
	~Skill() {}

	virtual void PerfMagicSkill(){}
	virtual void PerfNormalSkill(){}
	void SetProfession(Profession *profession) { m_profession = profession; }
protected:
	// 便于子类调用被修饰对象原有函数
	Profession* m_profession;
};
#endif