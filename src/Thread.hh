#ifndef REACTOR_THREAD_HEADER
#define REACTOR_THREAD_HEADER

#include "Runnable.hh"
#include "Noncopyable.hh"

class ThreadImpl;

class Thread : public Noncopyable {
	ThreadImpl *impl_;

public:
	Thread(Runnable &runnable);
	~Thread();

	bool hasFinished() const;
};

#endif // REACTOR_THREAD_HEADER
