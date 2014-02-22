#include <util/Wrapper.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <vector>

using namespace util;

namespace {

class Counter {
	int count_;

	Counter() : count_(0) {}

public:
	static Counter &instance() { static Counter instance; return instance; }

	void clear() { count_ = 0; }
	void prefix() { ++count_; }
	void suffix() { --count_; }
	int count() const { return count_; }
};

struct Prefix {
	void operator()() { Counter::instance().prefix(); }
};

struct Suffix {
	void operator()() { Counter::instance().suffix(); }
};

class A {
	int expected_;

public:
	A(int expected) : expected_(expected) {}

	void
	f()
	{
		CPPUNIT_ASSERT_EQUAL(expected_, Counter::instance().count());
	}
};

class WrapperTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(WrapperTester);
	CPPUNIT_TEST(testWrapped);
	CPPUNIT_TEST(testDirect);
	CPPUNIT_TEST(testOwned);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testWrapped()
	{
		A a(1);
		Wrapper<A, Prefix, Suffix> w(a, Prefix(), Suffix());
		w->f();
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testDirect()
	{
		A a(0);
		Wrapper<A, Prefix, Suffix> w(a, Prefix(), Suffix());
		w.direct().f();
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testOwned()
	{
		Wrapper<A, Prefix, Suffix> w(new A(1), Prefix(), Suffix());
		w->f();
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}
};

} // namespace

CPPUNIT_TEST_SUITE_REGISTRATION(WrapperTester);
