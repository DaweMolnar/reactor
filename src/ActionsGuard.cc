#include "ActionsGuard.hh"

void
ActionsGuard::release(Action *action)
{
	actions_.erase(action);
	delete action;
}

void
ActionsGuard::clear()
{
	for (Actions::iterator i(actions_.begin()); i != actions_.end(); ++i) {
		delete *i;
	}
	actions_.clear();
}

Action *
ActionsGuard::copy(const Action &action)
{
	Action *a = action.clone();
	actions_.insert(a);
	return a;
}
