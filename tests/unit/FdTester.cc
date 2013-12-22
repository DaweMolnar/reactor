#include <src/Fd.hh>

#include <tests/unit/mock/MockedFunction.hh>
#include <tests/unit/mock/MockRegistry.hh>
#include <tests/unit/mock/RedirectMockCFunction.h>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>
#include <queue>
#include <unistd.h>
#include <cstdarg>
#include <sstream>

REDIRECT_MOCK_C_FUNCTION1(close, void, int, fd)
REDIRECT_MOCK_C_FUNCTION3(read, ssize_t, int, fd, void *, buf, size_t, count)
REDIRECT_MOCK_C_FUNCTION3(write, ssize_t, int, fd, const void *, buf, size_t, count)

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
