#ifndef  __COMPOSITE_HPP_
#define __COMPOSITE_HPP_

#include <string>
#include <list>
#include "component.hpp"

class Composite : public Component
{
public:
	Composite(int type, std::string info) : m_component_type(type), m_component_info(info) {}
	~Composite() {}

	virtual bool AddComponent(Component* component);
	virtual bool RemoveComponent(Component* component);
	virtual bool ReplaceComponent(Component* component) { return false; }
	virtual void PrintInfo();
	virtual int GetComponentType() { return m_component_type; }

private:
	int m_component_type;
	std::string m_component_info;
	std::list<Component*> m_component_list;
};

bool Composite::AddComponent(Component* component)
{
	//这里注意子类的可见范围被缩减到基类的可见范围
	m_component_list.push_back(component);
	return true;
}

bool Composite::RemoveComponent(Component* component)
{
	for (std::list<Component*>::iterator it = m_component_list.begin(); it != m_component_list.end(); ++it)
	{
		if ((*it)->GetComponentType() == component->GetComponentType())
		{
			m_component_list.erase(it);
			return true;
		}
	}
	
	return false;
}

void Composite::PrintInfo()
{
	std::cout << m_component_info << std::endl;
	for (std::list<Component*>::iterator it = m_component_list.begin(); it != m_component_list.end(); ++it)
	{
		(*it)->PrintInfo();
	}
}

#endif
