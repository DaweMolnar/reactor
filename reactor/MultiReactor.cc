#include "MultiReactor.hh"

#include "Dispatcher.hh"

#include <thread/Pool.hh>
#include <thread/Guard.hh>

#include <cstdlib>
#include <stdexcept>

using namespace reactor;

MultiReactor::MultiReactor(size_t threadCount)
: threadCount_(threadCount)
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
	Dispatcher &dispatcher = Dispatcher::instance();
	bool iNeedToWait = false;
	std::unique_ptr<Dispatcher::FdEvents> fdEvents;
	std::unique_ptr<Backlog::Job> job;
	std::unique_ptr<util::DiffTime> remaining;

	while (true) {
		if (iNeedToWait) {
			fdEvents.reset(dispatcher.wait(remaining.release()));
		}
		if (job.get()) {
			job->execute();
		}

		{
			thread::Guard<thread::Mutex> guard(mutex_);
			if (job.get()) {
				job.reset(0);
				dispatcher.notify();
			}
			if (quit_) {
				break;
			}
			if (fdEvents.get()) {
				dispatcher.collectEvents(fdEvents.release());
				isLeaderWaiting_ = iNeedToWait = false;
				isLeaderWaitingCond_.notifyAll();
			}
			if (dispatcher.hasPendingEvents()) {
				job.reset(dispatcher.dequeueEvent());
			} else if (!isLeaderWaiting_) {
				isLeaderWaiting_ = iNeedToWait = true;
				remaining.reset(dispatcher.remaining());
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
