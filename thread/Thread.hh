#ifndef REACTOR_THREAD_THREAD_HEADER
#define REACTOR_THREAD_THREAD_HEADER

#include <util/Runnable.hh>
#include <util/Noncopyable.hh>

namespace thread {

class ThreadImpl;

class Thread : public util::Noncopyable {
	ThreadImpl *impl_;

public:
	Thread(util::Runnable &runnable);
	~Thread();

	bool hasFinished() const;
};

} // namespace thread

#endif // REACTOR_THREAD_THREAD_HEADER
