#include <src/DiffTime.hh>

#include <cppunit/extensions/HelperMacros.h>

class DiffTimeTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DiffTimeTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testFromRaw);
	CPPUNIT_TEST(testFromMs);
	CPPUNIT_TEST(testPositive);
	CPPUNIT_TEST(testLosslessness);
	CPPUNIT_TEST(testBoundaries);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		DiffTime dt;
	}

	void
	testFromRaw()
	{
		int64_t raw = (int64_t)1 << 32;
		DiffTime dt = DiffTime::raw(raw);
		CPPUNIT_ASSERT_EQUAL((int64_t)1 << 32, dt.raw());
		CPPUNIT_ASSERT_EQUAL(1000, dt.ms());
	}

	void
	testFromMs()
	{
		DiffTime dt = DiffTime::ms(1);
		CPPUNIT_ASSERT_EQUAL((int64_t)4294968, dt.raw());
		CPPUNIT_ASSERT_EQUAL(1, dt.ms());
	}

	void
	testPositive()
	{
		DiffTime dt = DiffTime::ms(+1);
		CPPUNIT_ASSERT_EQUAL(true, dt.positive());
		dt = DiffTime::ms(0);
		CPPUNIT_ASSERT_EQUAL(false, dt.positive());
		dt = DiffTime::ms(-1);
		CPPUNIT_ASSERT_EQUAL(false, dt.positive());

		dt = DiffTime::raw(+1);
		CPPUNIT_ASSERT_EQUAL(true, dt.positive());
		dt = DiffTime::raw(0);
		CPPUNIT_ASSERT_EQUAL(false, dt.positive());
		dt = DiffTime::raw(-1);
		CPPUNIT_ASSERT_EQUAL(false, dt.positive());
	}

	void
	testLosslessness()
	{
		for (int64_t i = -65536; i <= 65536; ++i) {
			CPPUNIT_ASSERT_EQUAL(i, DiffTime::raw(i).raw());
		}
		for (int32_t i = -65536; i <= 65536; ++i) {
			CPPUNIT_ASSERT_EQUAL(i, DiffTime::ms(i).ms());
		}
	}

	void
	testBoundaries()
	{
		DiffTime min = DiffTime::raw(INT64_MIN);
		DiffTime minusOne = DiffTime::raw(-1);
		DiffTime zero = DiffTime::raw(0);
		DiffTime one = DiffTime::raw(1);
		DiffTime max = DiffTime::raw(INT64_MAX);

		CPPUNIT_ASSERT_EQUAL(INT64_MIN, min.raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)-1, minusOne.raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)0, zero.raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)1, one.raw());
		CPPUNIT_ASSERT_EQUAL(INT64_MAX, max.raw());

		CPPUNIT_ASSERT_EQUAL(0, min.ms()); // overflow
		CPPUNIT_ASSERT_EQUAL(-1, minusOne.ms());
		CPPUNIT_ASSERT_EQUAL(0, zero.ms());
		CPPUNIT_ASSERT_EQUAL(0, one.ms());
		CPPUNIT_ASSERT_EQUAL(-1, max.ms()); // overflow

//TODO	CPPUNIT_ASSERT_EQUAL(INT32_MIN, DiffTime::ms(INT32_MIN).ms());
		CPPUNIT_ASSERT_EQUAL(INT32_MAX, DiffTime::ms(INT32_MAX).ms());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DiffTimeTester);
