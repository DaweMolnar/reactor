#include <src/Timers.hh>
#include <src/ActionMethod.hh>

#include <tests/unit/mock/Mocked.hh>
#include <tests/unit/mock/MockRegistry.hh>

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

	static Time
	now()
	{
		Mocked &m = MockRegistry::find("now");
		return Time::raw(m.expectedInt());
	}

	void
	testFireAllButUnexpired()
	{
		Mocked now("now");
		ActionsGuard ag;
		Timers t(ag, &TimersTester::now);
		DiffTime dt;

		t.add(Timer(DiffTime::raw(2), 2, Time::raw(0)), genActionMethod(*this, &TimersTester::action1));
		t.add(Timer(DiffTime::raw(3), 1, Time::raw(0)), genActionMethod(*this, &TimersTester::action2));

		actionCount1_ = actionCount2_ = 0;
		now.expect(0);
		CPPUNIT_ASSERT_EQUAL(true, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount2_);
		CPPUNIT_ASSERT_EQUAL((int64_t)2, dt.raw());

		actionCount1_ = actionCount2_ = 0;
		now.expect(2);
		now.expect(2);
		CPPUNIT_ASSERT_EQUAL(true, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount2_);
		CPPUNIT_ASSERT_EQUAL((int64_t)1, dt.raw());

		actionCount1_ = actionCount2_ = 0;
		now.expect(3);
		now.expect(3);
		CPPUNIT_ASSERT_EQUAL(true, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount2_);
		CPPUNIT_ASSERT_EQUAL((int64_t)1, dt.raw());

		actionCount1_ = actionCount2_ = 0;
		now.expect(4);
		CPPUNIT_ASSERT_EQUAL(false, t.fireAllButUnexpired(&dt));
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount1_);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actionCount2_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimersTester);
