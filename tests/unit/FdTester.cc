#include <src/Fd.hh>

#include <tests/unit/mock/MockedFunction.hh>
#include <tests/unit/mock/unistd.h>

#include <cppunit/extensions/HelperMacros.h>

class FdTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FdTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		MOCK_FUNCTION_DEFAULT(close);
		Fd fd(42);
		close->expect(42);
	}

	void
	testRead()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(read);
		Fd fd(43);
		char buf[23];

		close->expect(43);
		read->expectf("%d%p%d%d", 43, (void *)buf, (int)sizeof(buf), 45);
		CPPUNIT_ASSERT_EQUAL((size_t)45, fd.read(buf, sizeof(buf)));
	}

	void
	testWrite()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(write);
		Fd fd(78);
		char buf[6];

		close->expect(78);
		write->expectf("%d%p%d%d", 78, (void *)buf, (int)sizeof(buf), (int)sizeof(buf));
		CPPUNIT_ASSERT_EQUAL(sizeof(buf), fd.write(buf, sizeof(buf)));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FdTester);
