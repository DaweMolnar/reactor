#ifndef REACTOR_PRIMITIVE_CAST_EXCEPTION_HEADER
#define REACTOR_PRIMITIVE_CAST_EXCEPTION_HEADER

#include <typeinfo> // std::bad_cast

template <typename F, typename T>
class PrimitiveCastException : public std::bad_cast {
public:
	virtual ~PrimitiveCastException() throw() {}

	virtual const char *what() const throw() { return "incompatible types"; }
};

#endif // REACTOR_PRIMITIVE_CAST_EXCEPTION_HEADER
