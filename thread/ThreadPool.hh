#ifndef REACTOR_THREAD_THREADPOOL_HEADER
#define REACTOR_THREAD_THREADPOOL_HEADER

#include <thread/Thread.hh>

#include <vector>
#include <cstddef>

class ThreadPool {
	typedef std::vector<Thread *> Threads;

	util::Runnable &runnable_;
	Threads threads_;

public:
	ThreadPool(util::Runnable &runnable, size_t count);
	~ThreadPool();
};

#endif // REACTOR_THREAD_THREADPOOL_HEADER
