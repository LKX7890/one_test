#ifndef __RAND_ACTIVITY_HPP_
#define __RAND_ACTIVITY_HPP_

#include "Activity.hpp"

class RandActivity : public Activity
{
public:
	enum RAND_ACTIVITY_STATUS
	{
		RAND_ACTIVITY_STATUS_BEGIN,
		RAND_ACTIVITY_STATUS_END,
	};

	RandActivity(){}
	~RandActivity() {}
};
#endif