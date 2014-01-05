#include "MultiReactor.hh"

#include "ThreadPool.hh"
#include "Guard.hh"

#include <cstdlib>
#include <stdexcept>

MultiReactor::MultiReactor(Dispatcher &dispatcher, size_t threadCount)
: Reactor(dispatcher)
, threadCount_(threadCount)
, cond_(mutex_)
{
	if (threadCount_ <= 1) {
		throw std::runtime_error("there must be more threads for a MultiReactor");
	}
}

void
MultiReactor::run()
{
	while (true) {
		Backlog::Job *job = 0;
		{
			Guard<ThreadMutex> guard(mutex_);
			if (quit_) {
				break;
			}
			if (dispatcher_.hasPendingEvents()) {
				job = dispatcher_.dequeueEvent();
			} else {
				dispatcher_.collectEvents();
			}
		}
		if (job) {
			job->execute();
			dispatcher_.notify();
			delete job;
		}
	}
}

int
MultiReactor::loop()
{
	ThreadPool tp(*this, threadCount_ - 1);
	run();

	return EXIT_SUCCESS;
}
