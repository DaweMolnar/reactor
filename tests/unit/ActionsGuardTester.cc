#include <src/ActionsGuard.hh>
#include <src/ActionMethod.hh>

#include <cppunit/extensions/HelperMacros.h>

class ActionsGuardTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ActionsGuardTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testCopy);
	CPPUNIT_TEST(testRelease);
	CPPUNIT_TEST_SUITE_END();

	bool actionPerformed_;

public:
	void
	setUp()
	{
		actionPerformed_ = false;
	}

	void
	testConstruction()
	{
		ActionsGuard ag;
	}

	void
	action()
	{
		actionPerformed_ = true;
	}

	void
	testCopy()
	{
		ActionsGuard ag;
		Action *a = ag.copy(genActionMethod(*this, &ActionsGuardTester::action));
		CPPUNIT_ASSERT(a);
		a->perform();
		CPPUNIT_ASSERT_EQUAL(true, actionPerformed_);
	}

	void
	testRelease()
	{
		ActionsGuard ag;
		Action *a = ag.copy(genActionMethod(*this, &ActionsGuardTester::action));
		CPPUNIT_ASSERT(a);
		a->perform();
		CPPUNIT_ASSERT_EQUAL(true, actionPerformed_);
		ag.release(a);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ActionsGuardTester);
