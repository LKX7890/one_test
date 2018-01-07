#ifndef __MONSTER_STATIC_HPP_
#define __MONSTER_STATIC_HPP_

#include "monster.hpp"

class MonsterStatic : public Monster
{
public:
	MonsterStatic(int ai) : Monster(ai){}
	~MonsterStatic() {}

	virtual void OnEnterAOI();
};

void MonsterStatic::OnEnterAOI()
{
	std::cout << "stay here !!!" << std::endl;
}


#endif
