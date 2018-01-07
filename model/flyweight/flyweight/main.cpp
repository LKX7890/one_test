
#include "scene.hpp"

int main()
{
	Pos pos1(1, 2);
	Pos pos2(3, 4);
	Pos pos3(1, 5);
	Pos pos4(2, 2);
	
	Scene *scene = new Scene;
	scene->SetMonster(Monster::MONSTER_AI_TYPE_ACTIVE, pos1);
	scene->SetMonster(Monster::MONSTER_AI_TYPE_ACTIVE, pos2);
	scene->SetMonster(Monster::MONSTER_AI_TYPE_STATIC, pos3);
	scene->SetMonster(Monster::MONSTER_AI_TYPE_STATIC, pos4);

	scene->OnEnterAOI(pos1);
	scene->OnEnterAOI(pos4);

	system("pause");
	return 0;
}