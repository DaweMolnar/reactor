#ifndef REACTOR_RUNNABLE_HEADER
#define REACTOR_RUNNABLE_HEADER

struct Runnable {
	virtual ~Runnable() {}

	virtual void run() = 0;
};

#endif // REACTOR_RUNNABLE_HEADER
