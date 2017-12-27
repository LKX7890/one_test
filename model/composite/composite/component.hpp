#ifndef __COMPONENT_HPP_
#define __COMPONENT_HPP_

#include <iostream>

// �ṩ���ͻ�ʹ�õ�ͳһ�ӿ���
class Component
{
public:
	enum COMPONENT_TYPE_NAME
	{
		COMPONENT_TYPE_NAME_ROLE,
		COMPONENT_TYPE_NAME_TOUKUI,
		COMPONENT_TYPE_NAME_KUIJIA,
		COMPONENT_TYPE_NAME_TUIJIA,
		COMPONENT_TYPE_NAME_XIEZI,
		COMPONENT_TYPE_NAME_BAOSHI,
		COMPONENT_TYPE_NAME_FUWEN,
	};
	Component() {}
	virtual ~Component() {}

	virtual bool AddComponent(Component* component) { return false; }
	virtual bool RemoveComponent(Component* component) { return false; }
	virtual bool ReplaceComponent(Component* component) { return false; }
	virtual void PrintInfo() {}
	virtual int GetComponentType() { return -1; }
};

#endif
