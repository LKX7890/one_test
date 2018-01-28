
#include "randactivityobserver.hpp"
#include "Activity.hpp"
int main()
{
	RandActivity* randactivity = new RandActivity;
	RandActivityObserver* ra_observer = new RandActivityObserver;

	randactivity->AttachObserver(ra_observer);
	ra_observer->SetBeObserved(randactivity);

	randactivity->SetStatus(RandActivity::RAND_ACTIVITY_STATUS_BEGIN);
	randactivity->NotifyAllObserver();

	randactivity->SetStatus(RandActivity::RAND_ACTIVITY_STATUS_END);
	randactivity->NotifyAllObserver();

	system("pause");

	return 0;
}