#include <src/Fd.hh>

#include <src/RedirectMockCFunction.h>
#include <src/MockException.hh>
#include <src/PrimitiveCastException.hh>
#include <src/Primitive.hh>
#include <src/PrimitiveByFormatFactory.hh>
#include <src/Mocked.hh>
#include <src/MockRegistry.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>
#include <queue>
#include <unistd.h>
#include <cstdarg>
#include <sstream>

REDIRECT_MOCK_C_FUNCTION1(close, void, int, fd)
REDIRECT_MOCK_C_FUNCTION3(read, ssize_t, int, fd, void *, buf, size_t, count)
REDIRECT_MOCK_C_FUNCTION3(write, ssize_t, int, fd, const void *, buf, size_t, count)

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
