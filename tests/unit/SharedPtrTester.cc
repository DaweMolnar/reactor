#include <util/SharedPtr.hh>

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
	void ctor() { ++count_; }
	void dtor() { --count_; }
	int count() const { return count_; }
};

class A {
	int count_;

public:
	A() : count_(0) { Counter::instance().ctor(); }
	~A() { Counter::instance().dtor(); }

	void inc() { ++count_; }
	int count() const { return count_; }
};

class SharedPtrTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SharedPtrTester);
	CPPUNIT_TEST(testNoGuarding);
	CPPUNIT_TEST(testSimpleGuarding);
	CPPUNIT_TEST(testCopy);
	CPPUNIT_TEST(testOpEq);
	CPPUNIT_TEST(testOpEqSelf);
	CPPUNIT_TEST(testOpAsterisk);
	CPPUNIT_TEST(testOpArrow);
	CPPUNIT_TEST(testOrderInversion);
	CPPUNIT_TEST(testVector);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testNoGuarding()
	{
		{
			A a;
			SharedPtr<A> p(a);

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(&a, p.get());
			CPPUNIT_ASSERT_EQUAL(0, p.use_count());
			CPPUNIT_ASSERT_EQUAL(false, p.unique());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testSimpleGuarding()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(a, p.get());
			CPPUNIT_ASSERT_EQUAL(1, p.use_count());
			CPPUNIT_ASSERT_EQUAL(true, p.unique());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testCopy()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);
			SharedPtr<A> p2(p);

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(a, p.get());
			CPPUNIT_ASSERT_EQUAL(a, p2.get());
			CPPUNIT_ASSERT_EQUAL(2, p.use_count());
			CPPUNIT_ASSERT_EQUAL(2, p2.use_count());
			CPPUNIT_ASSERT_EQUAL(false, p.unique());
			CPPUNIT_ASSERT_EQUAL(false, p.unique());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testOpEq()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);
			SharedPtr<A> p2;

			p2 = p;

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(a, p.get());
			CPPUNIT_ASSERT_EQUAL(a, p2.get());
			CPPUNIT_ASSERT_EQUAL(2, p.use_count());
			CPPUNIT_ASSERT_EQUAL(2, p2.use_count());
			CPPUNIT_ASSERT_EQUAL(false, p.unique());
			CPPUNIT_ASSERT_EQUAL(false, p.unique());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testOpEqSelf()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);

			p = p;

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(a, p.get());
			CPPUNIT_ASSERT_EQUAL(1, p.use_count());
			CPPUNIT_ASSERT_EQUAL(true, p.unique());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testOpAsterisk()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);

			(*p).inc();
			CPPUNIT_ASSERT_EQUAL(1, (*p).count());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testOpArrow()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);

			p->inc();
			CPPUNIT_ASSERT_EQUAL(1, p->count());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testOrderInversion()
	{
		A *a = new A;
		SharedPtr<A> *p = new SharedPtr<A>(a);
		SharedPtr<A> *p2 = new SharedPtr<A>(*p);

		CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
		CPPUNIT_ASSERT_EQUAL(a, p->get());
		CPPUNIT_ASSERT_EQUAL(a, p2->get());
		CPPUNIT_ASSERT_EQUAL(2, p->use_count());
		CPPUNIT_ASSERT_EQUAL(2, p2->use_count());
		CPPUNIT_ASSERT_EQUAL(false, p->unique());
		CPPUNIT_ASSERT_EQUAL(false, p->unique());

		delete p;
		delete p2;

		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}

	void
	testVector()
	{
		{
			A *a = new A;
			SharedPtr<A> p(a);
			std::vector<SharedPtr<A> > pv;

			for (int i = 0; i < 100; ++i) {
				pv.push_back(p);
			}

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(a, pv[51].get());
			CPPUNIT_ASSERT_EQUAL(101, pv[64].use_count());
			CPPUNIT_ASSERT_EQUAL(101, pv[19].use_count());
			CPPUNIT_ASSERT_EQUAL(false, p.unique());
			CPPUNIT_ASSERT_EQUAL(false, pv[99].unique());

			pv.clear();

			CPPUNIT_ASSERT_EQUAL(1, Counter::instance().count());
			CPPUNIT_ASSERT_EQUAL(a, p.get());
			CPPUNIT_ASSERT_EQUAL(1, p.use_count());
			CPPUNIT_ASSERT_EQUAL(true, p.unique());
		}
		CPPUNIT_ASSERT_EQUAL(0, Counter::instance().count());
	}
};

} // namespace

CPPUNIT_TEST_SUITE_REGISTRATION(SharedPtrTester);
