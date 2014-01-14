#ifndef REACTOR_THREAD_HEADER
#define REACTOR_THREAD_HEADER

#include <util/Runnable.hh>
#include <util/Noncopyable.hh>

class ThreadImpl;

class Thread : public util::Noncopyable {
	ThreadImpl *impl_;

public:
	Thread(util::Runnable &runnable);
	~Thread();

	bool hasFinished() const;
};

#endif // REACTOR_THREAD_HEADER
