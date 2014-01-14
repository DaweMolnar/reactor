#ifndef REACTOR_THREAD_THREADIMPL_HEADER
#define REACTOR_THREAD_THREADIMPL_HEADER

class ThreadImpl {
public:
	virtual ~ThreadImpl() {}

	virtual bool hasFinished() const = 0;
};

#endif // REACTOR_THREAD_THREADIMPL_HEADER
