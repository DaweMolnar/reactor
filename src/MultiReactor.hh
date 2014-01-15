#ifndef REACTOR_MULTIREACTOR_HEADER
#define REACTOR_MULTIREACTOR_HEADER

#include "Reactor.hh"

#include <thread/ThreadCondition.hh>
#include <util/Runnable.hh>

#include <stddef.h>

class MultiReactor
: public Reactor
, public util::Runnable
{
	size_t threadCount_;
	thread::ThreadMutex mutex_;
	thread::ThreadCondition isLeaderWaitingCond_;
	volatile bool isLeaderWaiting_;

	virtual void run();

public:
	MultiReactor(Dispatcher &dispatcher, size_t threadCount);

	int loop();
};

#endif // REACTOR_MULTIREACTOR_HEADER
