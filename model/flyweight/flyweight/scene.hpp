#ifndef __SCENE_HPP_
#define __SCENE_HPP_
 
#include <map>
#include "monster.hpp"
#include "MonsterManager.hpp"

class Scene
{
public:
	Scene() {}
	~Scene() {}

	// 建立外部状态
	bool SetMonster(int type, Pos pos);

	// 内外状态交互(由外部条件访问内部状态，决定如何处理)
	void OnEnterAOI(Pos pos);

private:
	std::map<Pos, int> m_monster_map;
};

bool Scene::SetMonster(int type, Pos pos)
{
	MonsterManager* monster_manager = MonsterManager::GetInstall();
	if (NULL == monster_manager)
	{
		return false;
	}

	Monster *monster = monster_manager->CreateMonster(type);
	if (NULL == monster)
	{
		return false;
	}

	m_monster_map.insert(std::map<Pos, int>::value_type(pos, type));
	return true;
}

void Scene::OnEnterAOI(Pos pos)
{
	int monster_type = -1;
	std::map<Pos, int>::iterator it = m_monster_map.find(pos);
	if (it != m_monster_map.end())
	{
		monster_type = it->second;
	}
	else
	{
		return;
	}

	MonsterManager* monster_manager = MonsterManager::GetInstall();
	Monster *monster = monster_manager->GetMonster(monster_type);
	monster->OnEnterAOI();
}

#endif
