#include <src/Fd.hh>

#include <src/RedirectMockCFunction.h>
#include <src/MockException.hh>
#include <src/PrimitiveCastException.hh>
#include <src/Primitive.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>
#include <queue>
#include <unistd.h>
#include <cstdarg>
#include <sstream>

REDIRECT_MOCK_C_FUNCTION1(close, void, int, fd)
REDIRECT_MOCK_C_FUNCTION3(read, ssize_t, int, fd, void *, buf, size_t, count)
REDIRECT_MOCK_C_FUNCTION3(write, ssize_t, int, fd, const void *, buf, size_t, count)

class PrimitiveByFormatFactory {
public:
	static PrimitiveBase *create(const std::string &conversionSpecification, va_list ap);
};

PrimitiveBase *
PrimitiveByFormatFactory::create(const std::string &conversionSpecification, va_list ap)
{
	if (conversionSpecification == "d") {
		return new Integer(va_arg(ap, int));
	} else if (conversionSpecification == "p") {
		return new Pointer(va_arg(ap, void *));
	} else {
		throw std::runtime_error("unknown conversion specification (" + conversionSpecification + ")");
	}
}

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


class MockRegistry : public Noncopyable {
	typedef std::map<std::string, Mocked *> Mockeds;

	Mockeds mockeds_;

	MockRegistry() {}

	static MockRegistry &getInstance();

public:
	static void add(Mocked *mocked);
	static void remove(const Mocked *mocked);
	static Mocked &find(const std::string &name);
};

MockRegistry &
MockRegistry::getInstance()
{
	static MockRegistry instance;
	return instance;
}

void
MockRegistry::add(Mocked *mocked)
{
	MockRegistry &mr = getInstance();
	mr.mockeds_.insert(std::make_pair(mocked->name(), mocked));
}

void
MockRegistry::remove(const Mocked *mocked)
{
	MockRegistry &mr = getInstance();
	mr.mockeds_.erase(mocked->name());
}

Mocked &
MockRegistry::find(const std::string &name)
{
	MockRegistry &mr = getInstance();
	Mockeds::iterator i = mr.mockeds_.find(name);
	if (i != mr.mockeds_.end()) {
		return *(i->second);
	} else {
		throw MockException(name, "no such mock");
	}
}

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

template <typename F>
class MockedFunction : public Mocked {
	F *wrap_;
	F orig_;

public:
	MockedFunction(const std::string &name, F *wrap, F mock)
	: Mocked(name)
	, wrap_(wrap)
	, orig_(*wrap_)
	{
		*wrap_ = mock;
	}

	virtual
	~MockedFunction()
	{
		*wrap_ = orig_;
	}
};

template <typename F>
MockedFunction<F> *
createMockedFunction(const std::string &name, F *wrap, F mock)
{
	return new MockedFunction<F>(name, wrap, mock);
}

#define MOCK_FUNCTION(name, mock_name) \
	std::auto_ptr<Mocked> name(createMockedFunction(#name, &__wrap_ ## name ## _ptr, mock_name))

class FdTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FdTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST_SUITE_END();

	static void
	mock_close(int fd)
	{
		Mocked &m = MockRegistry::find("close");
		CPPUNIT_ASSERT_EQUAL(m.expectedInt(), fd);
	}

	static ssize_t
	mock_read(int fd, void *buf, size_t count)
	{
		Mocked &m = MockRegistry::find("read");
		CPPUNIT_ASSERT_EQUAL(m.expectedInt(), fd);
		CPPUNIT_ASSERT_EQUAL(m.expectedPointer(), (void *)buf);
		CPPUNIT_ASSERT_EQUAL(m.expectedInt(), (int)count);
		return m.expectedInt();
	}

public:
	void
	testConstruction()
	{
		MOCK_FUNCTION(close, mock_close);
		Fd fd(42);
		close->expect(42);
	}

	void
	testRead()
	{
		MOCK_FUNCTION(close, mock_close);
		MOCK_FUNCTION(read, mock_read);
		Fd fd(43);
		char buf[23];

		close->expect(43);
		read->expectf("%d%p%d%d", 43, (void *)buf, (int)sizeof(buf), 45);
		CPPUNIT_ASSERT_EQUAL((size_t)45, fd.read(buf, sizeof(buf)));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FdTester);
