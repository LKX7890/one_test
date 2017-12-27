#ifndef __LEAF_HPP_
#define __LEAF_HPP_

#include "component.hpp"

class Leaf : public Component
{
public:
	Leaf(int type, std::string info) : m_leaf_type(type), m_leaf_info(info) {}
	~Leaf() {}

	virtual bool AddComponent(Component* component) { return false; }
	virtual bool RemoveComponent(Component* component) { return false; }
	virtual bool ReplaceComponent(Component* component) { return false; }
	virtual void PrintInfo() { std::cout << m_leaf_info << std::endl; }
	virtual int GetComponentType() { return m_leaf_type; }
private:
	int m_leaf_type;
	std::string m_leaf_info;
};
#endif
