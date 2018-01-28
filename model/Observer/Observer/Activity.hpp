#ifndef __SUBJECT_HPP_
#define __SUBJECT_HPP_

#include <vector>
#include "observer.hpp"

class Activity
{
public:
	Activity(){}
	~Activity(){}

	void AttachObserver(Observer* observer) { m_observer_list.push_back(observer); }
	void NotifyAllObserver();
	void SetStatus(int status) { m_status = status; }
	int GetStatus() { return m_status; }

private:
	std::vector<Observer*> m_observer_list;
	int m_status;
};

void Activity::NotifyAllObserver()
{
	for (std::vector<Observer*>::iterator it = m_observer_list.begin(); it != m_observer_list.end(); ++it)
	{
		(*it)->Update();
	}
}

#endif