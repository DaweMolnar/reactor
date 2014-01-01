#ifndef BOUNDCOMMAND_HEADER
#define BOUNDCOMMAND_HEADER

#include "Command.hh"

#include <memory> // auto_ptr

template <typename R>
class BoundCommand : public Command0<R> {};

template <typename R, typename C = Command0<R> >
class BoundCommand0 : public BoundCommand<R> {
	C *c_;
	mutable bool own_;

public:
	BoundCommand0(const C &c)
	: c_(c.clone())
	, own_(true)
	{}

	BoundCommand0(const BoundCommand0 &orig)
	: c_(orig.c_)
	, own_(true)
	{
		orig.own_ = false;
	}

	~BoundCommand0()
	{
		if (own_) {
			delete c_;
		}
	}

	virtual BoundCommand0 *clone() const { return new BoundCommand0(*this); }
	virtual R execute() const { return c_->execute(); }
};

template <typename R, typename P1, typename C = Command1<R, P1> >
class BoundCommand1 : public BoundCommand<R> {
	C *c_;
	mutable bool own_;
	P1 p1_;

	BoundCommand1 &operator=(const BoundCommand1 &);

public:
	BoundCommand1(const C &c, P1 p1)
	: c_(c.clone())
	, own_(true)
	, p1_(p1)
	{}

	BoundCommand1(const BoundCommand1 &orig)
	: c_(orig.c_)
	, own_(true)
	, p1_(orig.p1_)
	{
		orig.own_ = false;
	}

	~BoundCommand1()
	{
		if (own_) {
			delete c_;
		}
	}

	virtual BoundCommand1 *clone() const { return new BoundCommand1(*this); }
	virtual R execute() const { return c_->execute(p1_); }
};

template <typename R>
BoundCommand0<R>
bindCommand(Command0<R> &c)
{
	return BoundCommand0<R>(c);
}

template <typename R, typename P1>
BoundCommand1<R, P1>
bindCommand(Command1<R, P1> &c, P1 p1)
{
	return BoundCommand1<R, P1>(c, p1);
}

template <typename R, typename P1>
BoundCommand1<R, P1, Command1<R, const P1 &> >
bindCommand(Command1<R, const P1 &> &c, P1 p1)
{
	return BoundCommand1<R, P1, Command1<R, const P1 &> >(c, p1);
}

#endif // BOUNDCOMMAND_HEADER
