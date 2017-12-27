
#include "component.hpp"
#include "composite.hpp"
#include "leaf.hpp"

int main()
{
	Component *role = new Composite(Component::COMPONENT_TYPE_NAME_ROLE, "��ɫ");
	Component *toukui = new Composite(Component::COMPONENT_TYPE_NAME_TOUKUI, "ͷ��");
	Component *kuijia = new Composite(Component::COMPONENT_TYPE_NAME_KUIJIA, "����");
	Component *tuijia = new Leaf(Component::COMPONENT_TYPE_NAME_TUIJIA, "�ȼ�");
	Component *xiezi = new Leaf(Component::COMPONENT_TYPE_NAME_XIEZI, "Ь��");

	Component *baoshi = new Leaf(Component::COMPONENT_TYPE_NAME_BAOSHI, "��ʯ");
	Component *fuwen = new Leaf(Component::COMPONENT_TYPE_NAME_FUWEN, "����");

	role->AddComponent(toukui);
	role->AddComponent(kuijia);
	role->AddComponent(tuijia);
	role->AddComponent(xiezi);

	toukui->AddComponent(fuwen);
	kuijia->AddComponent(baoshi);

	role->PrintInfo();

	role->RemoveComponent(kuijia);
	role->PrintInfo();

	system("pause");
	return 0;
}