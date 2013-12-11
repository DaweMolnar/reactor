#ifndef REACTOR_ACTIONMETHOD_HEADER
#define REACTOR_ACTIONMETHOD_HEADER

#include "Action.hh"

template <class T>
class ActionMethod : public Action {
public:
	typedef void (T::*Method)();

private:
	T &object_;
	Method method_;

public:
	ActionMethod(T &object, Method method)
	: object_(object)
	, method_(method)
	{}

	virtual Action *clone() const { return new ActionMethod<T>(*this); }

	virtual void perform() { (object_.*method_)(); }
};

template <class T>
ActionMethod<T>
genActionMethod(T &object, void (T::*method)())
{
	return ActionMethod<T>(object, method);
}

#endif // REACTOR_ACTIONMETHOD_HEADER
