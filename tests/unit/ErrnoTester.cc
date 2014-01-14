#include <util/ErrnoException.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <cerrno>

using namespace util;

class ErrnoTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ErrnoTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testDynamicConstruction);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testStrerrorFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		ErrnoException e("bind");
	}

	void
	testDynamicConstruction()
	{
		delete new ErrnoException("select");
	}

	void
	testGetters()
	{
		errno = EADDRINUSE;
		ErrnoException e("bind");

		CPPUNIT_ASSERT_EQUAL(EADDRINUSE, e.errorCode());
		CPPUNIT_ASSERT_EQUAL(std::string("bind"), e.name());
		// EADDRINUSE = 98
		CPPUNIT_ASSERT_EQUAL(std::string("bind: Address already in use (98)"), std::string(e.what()));
	}

	void
	testStrerrorFailure()
	{
		errno = -1;
		ErrnoException e("valami");

		CPPUNIT_ASSERT_EQUAL(-1, e.errorCode());
		CPPUNIT_ASSERT_EQUAL(std::string("valami"), e.name());
		// EINVAL = 22
		CPPUNIT_ASSERT_EQUAL(std::string("valami: [strerror_r() failure (22)] (-1)"), std::string(e.what()));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ErrnoTester);
