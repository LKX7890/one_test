#ifndef __RAND_ACTIVITY_OBSERVER_HPP_
#define __RAND_ACTIVITY_OBSERVER_HPP_

#include "observer.hpp"
#include "Activity.hpp"
#include "randactivity.hpp"

class RandActivityObserver : public Observer
{
public:
	RandActivityObserver() : m_activity_observed(NULL) {}
	~RandActivityObserver() {}

	void SetBeObserved(Activity* activity_observed) { m_activity_observed = activity_observed; }
	virtual void Update();

private:
	Activity* m_activity_observed;
};

void RandActivityObserver::Update()
{
	switch (m_activity_observed->GetStatus())
	{
		case RandActivity::RAND_ACTIVITY_STATUS_BEGIN:
		{
			std::cout << "活动开始" << std::endl;
		}
		break;

		case RandActivity::RAND_ACTIVITY_STATUS_END:
		{
			std::cout << "活动结束" << std::endl;
		}
		break;

		default:
		{
			std::cout << "Unknown Status" << std::endl;
		}
		break;
	}
}

#endif