#ifndef __MONSTER_MANAGER_HPP_
#define __MONSTER_MANAGER_HPP_

#include <map>
#include "monster.hpp"
#include "MonsterActive.hpp"
#include "MonsterStatic.hpp"

class MonsterManager
{
public:
	static MonsterManager* GetInstall();

	// 注册内部状态（基类）
	bool RegisterMonster(int type);

	// 访问内部状态
	Monster* CreateMonster(int type);
	Monster* GetMonster(int type);
	
private:
	MonsterManager() {}
	~MonsterManager() {}

	static MonsterManager *m_monster_manager;
	std::map<int, Monster*> m_monster_pool;
};

MonsterManager* MonsterManager::m_monster_manager = NULL;

MonsterManager* MonsterManager::GetInstall()
{
	if (NULL != m_monster_manager)
	{
		return m_monster_manager;
	}
	else
	{
		m_monster_manager = new MonsterManager;
		return m_monster_manager;
	}
}

bool MonsterManager::RegisterMonster(int type)
{
	Monster* monster = NULL;
	switch (type)
	{
	case Monster::MONSTER_AI_TYPE_ACTIVE:
	{
		monster = new MonsterActive(Monster::MONSTER_AI_TYPE_ACTIVE);
	}
	break;

	case  Monster::MONSTER_AI_TYPE_STATIC:
	{
		monster = new MonsterStatic(Monster::MONSTER_AI_TYPE_STATIC);
	}
	break;

	default:
	{
		std::cout << "Unknown monster type !!!" << std::endl;
	}
	break;
	}
	if (NULL == monster)
	{
		return false;
	}

	m_monster_pool.insert(std::map<int, Monster*>::value_type(type, monster));
	return true;
}

Monster* MonsterManager::CreateMonster(int type)
{
	std::map<int, Monster*>::iterator it = m_monster_pool.find(type);
	if (it != m_monster_pool.end())
	{
		return it->second;
	}
	else
	{
		bool succ = this->RegisterMonster(type);
		if (!succ)
		{
			return NULL;
		}

		return m_monster_pool[type];
	}
}

Monster* MonsterManager::GetMonster(int type)
{
	std::map<int, Monster*>::iterator it = m_monster_pool.find(type);
	if (it != m_monster_pool.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

#endif
