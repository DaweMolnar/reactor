#ifndef REACTOR_UTIL_COMMAND_HEADER
#define REACTOR_UTIL_COMMAND_HEADER

namespace util {

template <typename R>
class Command0 {
public:
	virtual ~Command0() {}
	virtual Command0 *clone() const = 0;
	virtual R execute() const = 0;
};

template <typename R, typename P1>
class Command1 {
public:
	virtual ~Command1() {}
	virtual Command1 *clone() const = 0;
	virtual R execute(P1) const = 0;
};

template <typename R, class T>
class MethodCommand0 : public Command0<R> {
	typedef R (T::*M)();

	T &t_;
	M m_;

public:
	MethodCommand0(T &t, M m) : t_(t), m_(m) {}

	virtual MethodCommand0 *clone() const { return new MethodCommand0(*this); }

	virtual R execute() const { return (t_.*m_)(); }
};

template <typename R, class T, typename P1>
class MethodCommand1 : public Command1<R, P1> {
	typedef R (T::*M)(P1);

	T &t_;
	M m_;

public:
	MethodCommand1(T &t, M m) : t_(t), m_(m) {}

	virtual MethodCommand1 *clone() const { return new MethodCommand1(*this); }

	virtual R execute(P1 p1) const { return (t_.*m_)(p1); }
};

template <typename R, class T>
MethodCommand0<R, T>
commandForMethod(T &t, R (T::*m)())
{
	return MethodCommand0<R, T>(t, m);
}

template <typename R, class T, typename P1>
MethodCommand1<R, T, P1>
commandForMethod(T &t, R (T::*m)(P1))
{
	return MethodCommand1<R, T, P1>(t, m);
}

} // namespace util

#endif // REACTOR_UTIL_COMMAND_HEADER
