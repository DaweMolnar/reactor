#ifndef REACTOR_THREAD_THREADIMPL_HEADER
#define REACTOR_THREAD_THREADIMPL_HEADER

namespace thread {

class ThreadImpl {
public:
	virtual ~ThreadImpl() {}

	virtual bool hasFinished() const = 0;
};

} // namespace thread

#endif // REACTOR_THREAD_THREADIMPL_HEADER
