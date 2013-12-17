#include <src/Fd.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stack>
#include <unistd.h>

#define REDIRECT_MOCK_C_FUNCTION1(name, ret, t1, p1) \
	extern "C" { \
		ret __real_ ## name (t1); \
		ret (*__wrap_ ## name ## _ptr)(t1) = __real_ ## name; \
		ret __wrap_ ## name (t1 p1) { return __wrap_ ## name ## _ptr(p1); } \
	} // extern "C"

REDIRECT_MOCK_C_FUNCTION1(close, void, int, fd)

static void
mock_close(int fd)
{
	CPPUNIT_ASSERT_EQUAL(42, fd);
}

class Mocked {
public:
	virtual ~Mocked() {}
};

template <typename F>
class MockedFunction : public Mocked {
	F *wrap_;
	F orig_;

public:
	MockedFunction(F *wrap, F mock)
	: wrap_(wrap)
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
createMockedFunction(F *wrap, F mock)
{
	return new MockedFunction<F>(wrap, mock);
}

#define MOCK_FUNCTION(name) \
	std::auto_ptr<Mocked> name(createMockedFunction(&__wrap_ ## name ## _ptr, mock_ ## name))

class FdTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FdTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		MOCK_FUNCTION(close);
		Fd fd(42);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(FdTester);
