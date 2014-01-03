#include "ThreadPool.hh"

ThreadPool::ThreadPool(Runnable &runnable, size_t count)
: runnable_(runnable)
{
	for (size_t i = 0; i < count; ++i) {
		threads_.push_back(new Thread(runnable_));
	}
}

ThreadPool::~ThreadPool()
{
	for (Threads::const_iterator i(threads_.begin()); i != threads_.end(); ++i) {
		delete *i;
	}
}
