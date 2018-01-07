#ifndef __MONSTER_ACTIVE_HPP_
#define __MONSTER_ACTIVE_HPP_

#include "monster.hpp"

class MonsterActive : public Monster
{
public:
	MonsterActive(int ai) : Monster(ai){}
	~MonsterActive() {}

	virtual void OnEnterAOI();
};

void MonsterActive::OnEnterAOI()
{
	std::cout << "move and attack incomer!!!" << std::endl;
}

#endif
