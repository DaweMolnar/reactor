#include "Mocked.hh"

#include "MockRegistry.hh"
#include "MockException.hh"
#include "PrimitiveByFormatFactory.hh"
#include "PrimitiveCastException.hh"

#include <sstream>
#include <iostream>

Mocked::Mocked(const std::string &name)
: name_(name)
{
	MockRegistry::add(this);
}

Mocked::~Mocked()
{
	MockRegistry::remove(this);
	if (!queue_.empty()) {
		MockException e(name_, "expected call is missing");
		if (std::uncaught_exception()) {
			std::cerr << e.what() << std::endl;
		} else {
			// XXX: throw in destructor?
			throw e;
		}
	}
}

void
Mocked::expectByConversionSpecification(const std::string &conversionSpecifier, va_list ap)
{
	queue_.push(PrimitiveByFormatFactory::create(conversionSpecifier, ap));
}

void
Mocked::vexpectf(const char *format, va_list ap)
{
	std::ostringstream oss;

	for (int i = 0; format[i]; ++i) {
		if (format[i] == '%') {
			if (oss.tellp()) {
				expectByConversionSpecification(oss.str(), ap);
			}
			oss.str(std::string());
			oss.clear();
		} else {
			oss << format[i];
		}
	}
	if (oss.tellp()) {
		expectByConversionSpecification(oss.str(), ap);
	}
}

void
Mocked::expectf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vexpectf(format, ap);
	va_end(ap);
}

class IntegerVisitor : public PrimitiveVisitor {
	int result_;

public:
	IntegerVisitor() : result_(0) {}

	virtual void visit(Integer &integer);
	virtual void visit(Pointer &pointer) { (void)pointer; throw PrimitiveCastException<Pointer, int>(); }

	int result() const { return result_; }
};

void
IntegerVisitor::visit(Integer &integer)
{
	result_ = integer.value();
}

int
Mocked::expectedInt()
{
	if (queue_.empty()) {
		throw MockException(name_, "unexpected call");
	}
	IntegerVisitor iv;
	PrimitiveBase *value = queue_.front();
	value->accept(iv);
	delete value;
	queue_.pop();
	return iv.result();
}

class PointerVisitor : public PrimitiveVisitor {
	void *result_;

public:
	PointerVisitor() : result_(0) {}

	virtual void visit(Integer &integer) { (void)integer; throw PrimitiveCastException<Integer, void *>(); }
	virtual void visit(Pointer &pointer);

	void *result() const { return result_; }
};

void
PointerVisitor::visit(Pointer &pointer)
{
	result_ = pointer.value();
}

void *
Mocked::expectedPointer()
{
	if (queue_.empty()) {
		throw MockException(name_, "unexpected call");
	}
	PointerVisitor pv;
	PrimitiveBase *value = queue_.front();
	value->accept(pv);
	delete value;
	queue_.pop();
	return pv.result();
}
