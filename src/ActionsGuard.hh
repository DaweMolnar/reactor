#ifndef REACTOR_ACTIONSGUARD_HEADER
#define REACTOR_ACTIONSGUARD_HEADER

#include "Noncopyable.hh"
#include "Action.hh"

#include <set>

class ActionsGuard : public Noncopyable {
	typedef std::set<Action *> Actions;

	Actions actions_;

public:
	~ActionsGuard() { clear(); }

	Action *copy(const Action &action);
	void release(Action *action);
	void clear();
};

#endif // REACTOR_ACTIONSGUARD_HEADER
