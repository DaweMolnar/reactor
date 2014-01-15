#ifndef REACTOR_THREAD_MUTEX_HEADER
#define REACTOR_THREAD_MUTEX_HEADER

#include <util/Noncopyable.hh>

namespace thread {

class MutexImpl;

class Mutex : public util::Noncopyable {
	MutexImpl *impl_;

	friend class Condition;

public:
	Mutex();
	~Mutex();

	void acquire();
	void release();
};

} // namespace thread

#endif // REACTOR_THREAD_MUTEX_HEADER
