#ifndef __OBSERVER_HPP_
#define __OBSERVER_HPP_

#include <iostream>

class Observer
{
public:
	Observer(){}
	~Observer(){}

	virtual void Update() {}
};
#endif