#ifndef REACTOR_THREADMUTEXIMPL_HEADER
#define REACTOR_THREADMUTEXIMPL_HEADER

namespace thread {

class ThreadMutexImpl {
public:
	virtual ~ThreadMutexImpl() {}

	virtual void acquire() = 0;
	virtual void release() = 0;
};

} // namespace thread

#endif // REACTOR_THREADMUTEXIMPL_HEADER
