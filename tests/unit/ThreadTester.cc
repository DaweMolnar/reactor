#include <src/Thread.hh>

#include <cppunit/extensions/HelperMacros.h>

class ThreadTester
: public CppUnit::TestFixture
, public Runnable
{
	CPPUNIT_TEST_SUITE(ThreadTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST_SUITE_END();

	volatile int count_;

public:
	void
	setUp()
	{
		count_ = 0;
	}

	virtual void
	run()
	{
		++count_;
	}

	void
	testConstruction()
	{
		{
			Thread th(*this);
		}
		CPPUNIT_ASSERT_EQUAL(1, (int)count_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadTester);
