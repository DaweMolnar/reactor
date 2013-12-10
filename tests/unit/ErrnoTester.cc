#include <src/ErrnoException.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <cerrno>

class ErrnoTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ErrnoTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		ErrnoException e("bind");
	}

	void
	testGetters()
	{
		errno = EADDRINUSE;
		ErrnoException e("bind");

		CPPUNIT_ASSERT_EQUAL(EADDRINUSE, e.errorCode());
		CPPUNIT_ASSERT_EQUAL(std::string("bind"), e.name());
		CPPUNIT_ASSERT_EQUAL(std::string("bind: Address already in use (98)"), std::string(e.what()));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ErrnoTester);
