#ifndef REACTOR_REACTOR_MULTIREACTOR_REACTOR_HEADER
#define REACTOR_REACTOR_MULTIREACTOR_REACTOR_HEADER

#include <reactor/Reactor.hh>

#include <thread/Condition.hh>
#include <util/Runnable.hh>

#include <stddef.h>

class MultiReactor
: public Reactor
, public util::Runnable
{
	size_t threadCount_;
	thread::Mutex mutex_;
	thread::Condition isLeaderWaitingCond_;
	volatile bool isLeaderWaiting_;

	virtual void run();

public:
	MultiReactor(Dispatcher &dispatcher, size_t threadCount);

	int loop();
};

#endif // REACTOR_REACTOR_MULTIREACTOR_REACTOR_HEADER
