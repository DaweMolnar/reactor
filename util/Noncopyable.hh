#ifndef REACTOR_NONCOPYABLE_HEADER
#define REACTOR_NONCOPYABLE_HEADER

class Noncopyable {
	Noncopyable(const Noncopyable &);
	Noncopyable &operator=(const Noncopyable &);

public:
	Noncopyable(void) {}
};

#endif // REACTOR_NONCOPYABLE_HEADER
