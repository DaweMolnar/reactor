#ifndef REACTOR_MULTIREACTOR_HEADER
#define REACTOR_MULTIREACTOR_HEADER

#include "Reactor.hh"
#include "Runnable.hh"
#include "ThreadCondition.hh"

#include <stddef.h>

class MultiReactor
: public Reactor
, public Runnable
{
	size_t threadCount_;
	ThreadMutex mutex_;
	ThreadCondition isLeaderWaitingCond_;
	volatile bool isLeaderWaiting_;

	virtual void run();

public:
	MultiReactor(Dispatcher &dispatcher, size_t threadCount);

	int loop();
};

#endif // REACTOR_MULTIREACTOR_HEADER
