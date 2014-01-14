#include <src/Timer.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>

using namespace util;

class TimerTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TimerTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testLess);
	CPPUNIT_TEST(testIterations);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		Timer t(DiffTime::raw(1), 0, Time::raw(0));
	}

	void
	testGetters()
	{
		Timer t(DiffTime::raw(42), 4, Time::raw(3));

		CPPUNIT_ASSERT_EQUAL((int64_t)42, t.interval().raw());
		CPPUNIT_ASSERT_EQUAL((uint64_t)45, t.expiration().raw());
		CPPUNIT_ASSERT_EQUAL((size_t)0, t.iterationCount());
		CPPUNIT_ASSERT_EQUAL((size_t)4, t.iterationLimit());
	}

	void
	testLess()
	{
		Timer a(DiffTime::raw(1), 0, Time::raw(0));
		Timer b(DiffTime::raw(2), 0, Time::raw(0));
		Timer c(DiffTime::raw(2), 0, Time::raw(0));

		CPPUNIT_ASSERT_EQUAL(true, a < b);
		CPPUNIT_ASSERT_EQUAL(true, a < c);
		CPPUNIT_ASSERT_EQUAL(false, b < a);
		CPPUNIT_ASSERT_EQUAL(false, c < a);
		CPPUNIT_ASSERT_EQUAL(false, a < a);
		CPPUNIT_ASSERT_EQUAL(false, b < b);
		CPPUNIT_ASSERT_EQUAL(false, c < c);
		CPPUNIT_ASSERT_EQUAL(false, b < c);
		CPPUNIT_ASSERT_EQUAL(false, c < b);
	}

	void
	testIterations()
	{
		Timer t(DiffTime::raw(3), 2, Time::raw(0));

		CPPUNIT_ASSERT_EQUAL((size_t)0, t.iterationCount());
		CPPUNIT_ASSERT_EQUAL(true, t.hasRemainingIterations());
		CPPUNIT_ASSERT_NO_THROW(t.fire());

		CPPUNIT_ASSERT_EQUAL((size_t)1, t.iterationCount());
		CPPUNIT_ASSERT_EQUAL(true, t.hasRemainingIterations());
		CPPUNIT_ASSERT_NO_THROW(t.fire());

		CPPUNIT_ASSERT_EQUAL((size_t)2, t.iterationCount());
		CPPUNIT_ASSERT_EQUAL(false, t.hasRemainingIterations());
		CPPUNIT_ASSERT_THROW(t.fire(), std::runtime_error);

		CPPUNIT_ASSERT_EQUAL((size_t)2, t.iterationCount());
		CPPUNIT_ASSERT_EQUAL(false, t.hasRemainingIterations());
		CPPUNIT_ASSERT_THROW(t.fire(), std::runtime_error);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimerTester);
