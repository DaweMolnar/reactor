#ifndef REACTOR_ACTIONMETHOD_HEADER
#define REACTOR_ACTIONMETHOD_HEADER

#include "Action.hh"

template <class T, class M = void (T::*)()>
class ActionMethod : public Action {
	T &object_;
	M method_;

public:
	ActionMethod(T &object, M method)
	: object_(object)
	, method_(method)
	{}

	virtual Action *clone() const { return new ActionMethod<T, M>(*this); }

	virtual void perform() { (object_.*method_)(); }
};

template <class T, class M>
ActionMethod<T, M>
genActionMethod(T &object, M method)
{
	return ActionMethod<T, M>(object, method);
}

#endif // REACTOR_ACTIONMETHOD_HEADER
