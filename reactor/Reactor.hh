#ifndef REACTOR_REACTOR_REACTOR_REACTOR_HEADER
#define REACTOR_REACTOR_REACTOR_REACTOR_HEADER

namespace reactor {

class Reactor {
protected:
	volatile bool quit_;

public:
	Reactor()
	: quit_(false)
	{}

	void quit() { quit_ = true; }
	int loop();
};

} // namespace reactor

#endif // REACTOR_REACTOR_REACTOR_REACTOR_HEADER
