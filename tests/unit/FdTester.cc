#include <src/Fd.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>
#include <queue>
#include <unistd.h>

#define REDIRECT_MOCK_C_FUNCTION1(name, ret, t1, n1) \
	extern "C" { \
		ret __real_ ## name (t1); \
		ret (*__wrap_ ## name ## _ptr)(t1) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1) { return __wrap_ ## name ## _ptr(n1); } \
	} // extern "C"

#define REDIRECT_MOCK_C_FUNCTION2(name, ret, t1, n1, t2, n2) \
	extern "C" { \
		ret __real_ ## name (t1, t2); \
		ret (*__wrap_ ## name ## _ptr)(t1, t2) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1, t2 n2) { return __wrap_ ## name ## _ptr(n1, n2); } \
	} // extern "C"

#define REDIRECT_MOCK_C_FUNCTION3(name, ret, t1, n1, t2, n2, t3, n3) \
	extern "C" { \
		ret __real_ ## name (t1, t2, t3); \
		ret (*__wrap_ ## name ## _ptr)(t1, t2, t3) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1, t2 n2, t3 n3) { return __wrap_ ## name ## _ptr(n1, n2, n3); } \
	} // extern "C"

REDIRECT_MOCK_C_FUNCTION1(close, void, int, fd)
REDIRECT_MOCK_C_FUNCTION3(read, ssize_t, int, fd, void *, buf, size_t, count)
REDIRECT_MOCK_C_FUNCTION3(write, ssize_t, int, fd, const void *, buf, size_t, count)

class MockException : public std::runtime_error {
public:
	MockException(const std::string &name, const std::string &reason)
	: std::runtime_error(name + ": " + reason)
	{
	}
};

template <typename F, typename T>
class PrimitiveCastException : public std::bad_cast {
public:
	virtual ~PrimitiveCastException() throw() {}

	virtual const char *what() const throw() { return "incompatible types"; }
};

class Integer;
class Pointer;

class PrimitiveVisitor {
public:
	virtual ~PrimitiveVisitor() {}

	virtual void visit(Integer &integer) = 0;
	virtual void visit(Pointer &pointer) = 0;
};

class Primitive {
public:
	virtual ~Primitive() {}

	virtual void accept(PrimitiveVisitor &visitor) = 0;
};

class Integer : public Primitive {
	int value_;

public:
	explicit Integer(int value) : value_(value) {}

	int value() const { return value_; }
	virtual void accept(PrimitiveVisitor &visitor) { visitor.visit(*this); }
};

class Pointer : public Primitive {
	void *value_;

public:
	explicit Pointer(void *value) : value_(value) {}

	void *value() const { return value_; }
	virtual void accept(PrimitiveVisitor &visitor) { visitor.visit(*this); }
};

class Mocked : public Noncopyable {
	const std::string name_;
	std::queue<Primitive *> queue_;

public:
	Mocked(const std::string &name);
	virtual ~Mocked();

	const std::string &name() const { return name_; }

	void expect(int value);
	void expect(void *value);
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
Mocked::expect(int value)
{
	queue_.push(new Integer(value));
}

void
Mocked::expect(void *value)
{
	queue_.push(new Pointer(value));
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
	Primitive *value = queue_.front();
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
	Primitive *value = queue_.front();
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
		(void)buf;
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
		read->expect(43);
		read->expect((void *)buf);
		read->expect(sizeof(buf));
		read->expect(45);
		CPPUNIT_ASSERT_EQUAL((size_t)45, fd.read(buf, sizeof(buf)));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FdTester);
