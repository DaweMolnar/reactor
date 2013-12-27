#include <src/Timers.hh>
#include <src/ActionMethod.hh>

#include <tests/unit/mock/MockedFunction.hh>
#include <tests/unit/mock/time.h>

#include <cppunit/extensions/HelperMacros.h>

class TimersTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TimersTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testAddTimerActions);
	CPPUNIT_TEST(testFireAllButUnexpired);
	CPPUNIT_TEST_SUITE_END();

	size_t actionCount1_;
	size_t actionCount2_;

public:
	void
	setUp()
	{
		actionCount1_ = 0;
		actionCount2_ = 0;
	}

	void
	testConstruction()
	{
		ActionsGuard ag;
		Timers t(ag);
	}

	void
	action1()
	{
		++actionCount1_;
	}

	void
	action2()
	{
		++actionCount2_;
	}

	void
	testAddTimerActions()
	{
		ActionsGuard ag;
		Timers t(ag);

		t.add(Timer(DiffTime::raw(2), 0, Time::raw(0)), genActionMethod(*this, &TimersTester::action1));
		t.add(Timer(DiffTime::raw(3), 0, Time::raw(0)), genActionMethod(*this, &TimersTester::action2));
	}

	void
	testFireAllButUnexpired()
	{
		MOCK_FUNCTION_DEFAULT(gettimeofday);
		ActionsGuard ag;
		Timers t(ag);
		DiffTime dt;

		t.add(Timer(DiffTime::ms(2), 2, Time::raw(0)), genActionMethod(*this, &TimersTester::action1));
		t.add(Timer(DiffTime::ms(3), 1, Time::raw(0)), genActionMethod(*this, &TimersTester::action2));

		actionCount1_ = actionCount2_ = 0;
		gettimeofday->expectf("%d%d%d", 0, 0, 0);
		CPPUNIT_ASSERT_EQUAL(true, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount2_);
		CPPUNIT_ASSERT_EQUAL(2, dt.ms());

		actionCount1_ = actionCount2_ = 0;
		gettimeofday->expectf("%d%d%d", 0, 2000, 0);
		gettimeofday->expectf("%d%d%d", 0, 1999, 0);
		CPPUNIT_ASSERT_EQUAL(true, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount2_);
		CPPUNIT_ASSERT_EQUAL(1, dt.ms());

		actionCount1_ = actionCount2_ = 0;
		gettimeofday->expectf("%d%d%d", 0, 3000, 0);
		gettimeofday->expectf("%d%d%d", 0, 3000, 0);
		CPPUNIT_ASSERT_EQUAL(true, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount2_);
		CPPUNIT_ASSERT_EQUAL(1, dt.ms());

		actionCount1_ = actionCount2_ = 0;
		gettimeofday->expectf("%d%d%d", 0, 4000, 0);
		CPPUNIT_ASSERT_EQUAL(false, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount2_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimersTester);
