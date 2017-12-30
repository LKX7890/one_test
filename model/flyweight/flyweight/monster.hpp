#ifndef __MONSTER_HPP_
#define __MONSTER_HPP_

#include <iostream>
struct Pos
{
	Pos(int pos_x, int pos_y) : x(pos_x), y(pos_y) {}
	int x;
	int y;

	bool operator < (const Pos &o) const
	{
		return (x*100+y) < (o.x*100+o.y);
	}
};

class Monster
{
public:
	enum MONSTER_AI_TYPE
	{
		MONSTER_AI_TYPE_ACTIVE,
		MONSTER_AI_TYPE_STATIC,
	};

	Monster(int ai) : m_monste_ai_type(ai){}
	virtual ~Monster() {}

	virtual void OnEnterAOI(){ std::cout << "virtual function" << std::endl; }
	int GetAiType(){ return m_monste_ai_type; }
private:
	int m_monste_ai_type;
};
#endif
