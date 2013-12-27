#include <src/AutoFd.hh>

#include <src/ErrnoException.hh>

#include <tests/unit/mock/MockedFunction.hh>
#include <tests/unit/mock/unistd.h>

#include <cppunit/extensions/HelperMacros.h>

#include <fcntl.h>

class AutoFdTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(AutoFdTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testRelease);
	CPPUNIT_TEST(testReset);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		MOCK_FUNCTION_DEFAULT(close);
		AutoFd afd(42);
		close->expect(42);
	}

	void
	testRelease()
	{
		AutoFd afd(53);

		CPPUNIT_ASSERT_EQUAL(53, afd.release());
		CPPUNIT_ASSERT_EQUAL((int)AutoFd::INVALID, afd.release());
	}

	void
	testReset()
	{
		MOCK_FUNCTION_DEFAULT(close);
		AutoFd afd(33);

		close->expect(33);
		afd.reset(34);
		close->expect(34);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(AutoFdTester);
