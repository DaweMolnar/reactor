#ifndef REACTOR_THREAD_POOL_HEADER
#define REACTOR_THREAD_POOL_HEADER

#include <thread/Thread.hh>

#include <vector>
#include <cstddef>

namespace thread {

class Pool {
	typedef std::vector<Thread *> Threads;

	util::Runnable &runnable_;
	Threads threads_;

public:
	Pool(util::Runnable &runnable, size_t count);
	~Pool();
};

} // namespace thread

#endif // REACTOR_THREAD_POOL_HEADER
