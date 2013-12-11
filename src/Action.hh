#ifndef REACTOR_ACTION_HEADER
#define REACTOR_ACTION_HEADER

class Action {
public:
	virtual ~Action() {}
	virtual Action *clone() const = 0;

	virtual void perform() = 0;
};

#endif // REACTOR_ACTION_HEADER
