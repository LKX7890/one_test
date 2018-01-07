#include "Profession.hpp"
#include "Skill.hpp"
#include "magician.hpp"
#include "magicskill.h"
#include "NormalSkill.hpp"

int main()
{
	// 创建被修饰对象
	Profession * magician = new Magician;

	// 创建修饰对象
	Skill *magic_skill = new MagicSkill;
	Skill *normal_skill = new NormalSkill;

	// 设置修饰对象，在子类可以被修饰对象原来的函数接口，包含原来的能力
	magic_skill->SetProfession(magician);
	normal_skill->SetProfession(magician);

	// 动态添加新的能力
	magician = magic_skill;
	magician->PerfMagicSkill();	// 使用新功能

	magician = normal_skill;
	magician->PerfNormalSkill();

	system("pause");
	return 0;
}