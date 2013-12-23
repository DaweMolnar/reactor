#include <src/Time.hh>

#include <src/ErrnoException.hh>

#include <tests/unit/mock/MockedFunction.hh>
#include <tests/unit/mock/time.h>

#include <cppunit/extensions/HelperMacros.h>

class TimeTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TimeTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testGetters);
	CPPUNIT_TEST(testTimeSubTime);
	CPPUNIT_TEST(testTimeAddDiffTime);
	CPPUNIT_TEST(testTimeSubDiffTime);
	CPPUNIT_TEST(testLess);
	CPPUNIT_TEST(testNow);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		Time t;
	}

	void
	testGetters()
	{
		uint64_t raw = DiffTime::ms(43).raw();
		Time t = Time::raw(raw);

		CPPUNIT_ASSERT_EQUAL(raw, t.raw());
		CPPUNIT_ASSERT_EQUAL((uint32_t)43, t.msFraction());
		CPPUNIT_ASSERT_EQUAL((uint32_t)43000, t.usFraction());
		CPPUNIT_ASSERT_EQUAL((uint32_t)43000000, t.nsFraction());
	}

	void
	testTimeSubTime()
	{
		Time a = Time::raw(1000);
		Time b = Time::raw(1042);
		Time c = Time::raw(1042);

		CPPUNIT_ASSERT_EQUAL((int64_t)0, (a - a).raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)42, (b - a).raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)42, (c - a).raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)-42, (a - b).raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)-42, (a - c).raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)0, (b - c).raw());
	}

	void
	testTimeAddDiffTime()
	{
		Time t = Time::raw(1);
		DiffTime dt = DiffTime::raw(2);

		CPPUNIT_ASSERT_EQUAL((uint64_t)3, (t + dt).raw());
		t += dt;
		CPPUNIT_ASSERT_EQUAL((uint64_t)3, t.raw());
	}

	void
	testTimeSubDiffTime()
	{
		Time t = Time::raw(3);
		DiffTime dt = DiffTime::raw(2);

		CPPUNIT_ASSERT_EQUAL((uint64_t)1, (t - dt).raw());
		t -= dt;
		CPPUNIT_ASSERT_EQUAL((uint64_t)1, t.raw());
	}

	void
	testLess()
	{
		Time a = Time::raw(1);
		Time b = Time::raw(2);
		Time c = Time::raw(2);

		CPPUNIT_ASSERT_EQUAL(true, a < b);
		CPPUNIT_ASSERT_EQUAL(true, a < c);
		CPPUNIT_ASSERT_EQUAL(false, b < a);
		CPPUNIT_ASSERT_EQUAL(false, b < c);
		CPPUNIT_ASSERT_EQUAL(false, a < a);
		CPPUNIT_ASSERT_EQUAL(false, b < b);
		CPPUNIT_ASSERT_EQUAL(false, c < c);
	}

	void
	testNow()
	{
		MOCK_FUNCTION_DEFAULT(gettimeofday);
		Time t;

		gettimeofday->expectf("%d%d%d", 1, 2, -1);
		CPPUNIT_ASSERT_THROW(Time::now(), ErrnoException);
		gettimeofday->expectf("%d%d%d", 1, 2, 0);
		t = Time::now();
		CPPUNIT_ASSERT_EQUAL(((uint64_t)1 << 32) | (((uint64_t)2 << 32) / 1000000), t.raw());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeTester);
