
#include "component.hpp"
#include "composite.hpp"
#include "leaf.hpp"

int main()
{
	Component *role = new Composite(Component::COMPONENT_TYPE_NAME_ROLE, "½ÇÉ«");
	Component *toukui = new Composite(Component::COMPONENT_TYPE_NAME_TOUKUI, "Í·¿ø");
	Component *kuijia = new Composite(Component::COMPONENT_TYPE_NAME_KUIJIA, "¿ø¼×");
	Component *tuijia = new Leaf(Component::COMPONENT_TYPE_NAME_TUIJIA, "ÍÈ¼×");
	Component *xiezi = new Leaf(Component::COMPONENT_TYPE_NAME_XIEZI, "Ð¬×Ó");

	Component *baoshi = new Leaf(Component::COMPONENT_TYPE_NAME_BAOSHI, "±¦Ê¯");
	Component *fuwen = new Leaf(Component::COMPONENT_TYPE_NAME_FUWEN, "·ûÎÄ");

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