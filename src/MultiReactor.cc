#include "MultiReactor.hh"

#include <thread/Pool.hh>
#include <thread/Guard.hh>

#include <cstdlib>
#include <stdexcept>

MultiReactor::MultiReactor(Dispatcher &dispatcher, size_t threadCount)
: Reactor(dispatcher)
, threadCount_(threadCount)
, isLeaderWaitingCond_(mutex_)
, isLeaderWaiting_(false)
{
	if (threadCount_ <= 1) {
		throw std::runtime_error("there must be more threads for a MultiReactor");
	}
}

void
MultiReactor::run()
{
	bool iNeedToWait = false;
	std::auto_ptr<Dispatcher::FdEvents> fdEvents;
	std::auto_ptr<Backlog::Job> job;
	std::auto_ptr<util::DiffTime> remaining;

	while (true) {
		if (iNeedToWait) {
			fdEvents.reset(dispatcher_.wait(remaining.release()));
		}
		if (job.get()) {
			job->execute();
		}

		{
			thread::Guard<thread::Mutex> guard(mutex_);
			if (job.get()) {
				job.reset(0);
				dispatcher_.notify();
			}
			if (quit_) {
				break;
			}
			if (fdEvents.get()) {
				dispatcher_.collectEvents(fdEvents.release());
				isLeaderWaiting_ = iNeedToWait = false;
				isLeaderWaitingCond_.notifyAll();
			}
			if (dispatcher_.hasPendingEvents()) {
				job.reset(dispatcher_.dequeueEvent());
			} else if (!isLeaderWaiting_) {
				isLeaderWaiting_ = iNeedToWait = true;
				remaining.reset(dispatcher_.remaining());
			} else {
				isLeaderWaitingCond_.wait();
			}
		}
	}
}

int
MultiReactor::loop()
{
	thread::Pool tp(*this, threadCount_ - 1);
	run();

	return EXIT_SUCCESS;
}
