#include <util/DiffTime.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <limits>

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
		DiffTime min = DiffTime::raw(std::numeric_limits<int64_t>::min());
		DiffTime minusOne = DiffTime::raw(-1);
		DiffTime zero = DiffTime::raw(0);
		DiffTime one = DiffTime::raw(1);
		DiffTime max = DiffTime::raw(std::numeric_limits<int64_t>::max());

		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int64_t>::min(), min.raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)-1, minusOne.raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)0, zero.raw());
		CPPUNIT_ASSERT_EQUAL((int64_t)1, one.raw());
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int64_t>::max(), max.raw());

//		CPPUNIT_ASSERT_EQUAL(0, min.ms()); // overflow
		CPPUNIT_ASSERT_EQUAL(-1, minusOne.ms());
		CPPUNIT_ASSERT_EQUAL(0, zero.ms());
		CPPUNIT_ASSERT_EQUAL(0, one.ms());
//		CPPUNIT_ASSERT_EQUAL(-1, max.ms()); // overflow

		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int32_t>::min()+1, DiffTime::ms(std::numeric_limits<int32_t>::min()+1).ms());
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int32_t>::max(), DiffTime::ms(std::numeric_limits<int32_t>::max()).ms());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DiffTimeTester);
