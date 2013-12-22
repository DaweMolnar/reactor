#ifndef REACTOR_MOCKED_HEADER
#define REACTOR_MOCKED_HEADER

#include "Noncopyable.hh"
#include "Primitive.hh"

#include <string>
#include <queue>

class Mocked : public Noncopyable {
	const std::string name_;
	std::queue<PrimitiveBase *> queue_;

public:
	Mocked(const std::string &name);
	virtual ~Mocked();

	const std::string &name() const { return name_; }

	template <typename T>
	void
	expect(const T &value)
	{
		queue_.push(new Primitive<T>(value));
	}

	void expectByConversionSpecification(const std::string &conversionSpecification, va_list ap);
	void vexpectf(const char *format, va_list ap);
	void expectf(
	/* 1   hidden this */
	/*<2>*/const char *format,
	/*<3>*/...
	) __attribute__((format(printf, 2, 3)));

	int expectedInt();
	void *expectedPointer();
};

#endif // REACTOR_MOCKED_HEADER
