#ifndef REACTOR_PTHREAD_PTHREADIMPL_HEADER
#define REACTOR_PTHREAD_PTHREADIMPL_HEADER

#include "../ThreadImpl.hh"
#include "../Runnable.hh"

#include <pthread.h>

namespace pthread {

class PthreadImpl : public ThreadImpl {
	union Castor {
		void *pthreadArg;
		PthreadImpl *pthreadImpl;
	};

	pthread_t thread_;
	Runnable &runnable_;
	volatile bool finished_;

	static void *routine(void *arg);
	void run();

public:
	PthreadImpl(Runnable &runnable);
	~PthreadImpl();

	virtual bool hasFinished() const { return finished_; }
};

} // namespace pthread

#endif // REACTOR_PTHREAD_PTHREADIMPL_HEADER
