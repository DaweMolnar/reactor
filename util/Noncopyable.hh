#ifndef REACTOR_UTIL_NONCOPYABLE_HEADER
#define REACTOR_UTIL_NONCOPYABLE_HEADER

namespace util {

class Noncopyable {
	Noncopyable(const Noncopyable &);
	Noncopyable &operator=(const Noncopyable &);

public:
	Noncopyable(void) {}
};

} // namespace util

#endif // REACTOR_UTIL_NONCOPYABLE_HEADER
