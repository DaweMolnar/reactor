#ifndef REACTOR_UTIL_RUNNABLE_HEADER
#define REACTOR_UTIL_RUNNABLE_HEADER

namespace util {

struct Runnable {
	virtual ~Runnable() {}

	virtual void run() = 0;
};

} // namespace util

#endif // REACTOR_UTIL_RUNNABLE_HEADER
