#ifndef REACTOR_THREAD_HEADER
#define REACTOR_THREAD_HEADER

#include "Runnable.hh"
#include "Noncopyable.hh"

#include <memory> // std::auto_ptr

struct ThreadImpl;

class Thread : public Noncopyable {
	std::auto_ptr<ThreadImpl> impl_;
	Runnable &runnable_;

	static void *routine(void *arg);

public:
	Thread(Runnable &runnable);
	~Thread();
};

#endif // REACTOR_THREAD_HEADER
