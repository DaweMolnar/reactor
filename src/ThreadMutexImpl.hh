#ifndef REACTOR_THREADMUTEXIMPL_HEADER
#define REACTOR_THREADMUTEXIMPL_HEADER

class ThreadMutexImpl {
public:
	virtual ~ThreadMutexImpl() {}

	virtual void acquire() = 0;
	virtual void release() = 0;
};

#endif // REACTOR_THREADMUTEXIMPL_HEADER
