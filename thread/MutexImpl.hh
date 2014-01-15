#ifndef REACTOR_THREAD_MUTEXIMPL_HEADER
#define REACTOR_THREAD_MUTEXIMPL_HEADER

namespace thread {

class MutexImpl {
public:
	virtual ~MutexImpl() {}

	virtual void acquire() = 0;
	virtual void release() = 0;
};

} // namespace thread

#endif // REACTOR_THREAD_MUTEXIMPL_HEADER
