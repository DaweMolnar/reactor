#include <util/Wrapper.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <vector>

using namespace util;

namespace {

template <class T, class L>
class LockerWrapper : public Wrapper<T, typename L::Acquirer, typename L::Releaser> {
public:
	typedef typename L::Acquirer Acquirer;
	typedef typename L::Releaser Releaser;

	LockerWrapper(T& t, L& l)
	: Wrapper<T, Acquirer, Releaser>(t, Acquirer(l), Releaser(l))
	{}

	LockerWrapper(T* t, L& l)
	: Wrapper<T, Acquirer, Releaser>(t, Acquirer(l), Releaser(l))
	{}
};

class MyMutex {
public:
	MyMutex() : lockLevel_(0) {}

	void acquire() { ++lockLevel_; }
	void release() { --lockLevel_; }
	int lockLevel() const { return lockLevel_; }

	class Acquirer {
	public:
		Acquirer(MyMutex& mutex) : mutex_(&mutex) {}
		void operator()() { mutex_->acquire(); }

	private:
		MyMutex* mutex_;
	};

	class Releaser {
	public:
		Releaser(MyMutex& mutex) : mutex_(&mutex) {}
		void operator()() { mutex_->release(); }

	private:
		MyMutex* mutex_;
	};

private:
	int lockLevel_;
};

template <class T>
class MutexWrapper : public LockerWrapper<T, MyMutex> {
public:
	MutexWrapper(T& t) : LockerWrapper<T, MyMutex>(t, mutex_) {}
	MutexWrapper(T* t) : LockerWrapper<T, MyMutex>(t, mutex_) {}

	const MyMutex& mutex() const { return mutex_; }

private:
	MyMutex mutex_;
};

struct A {
	void f(int level, const MyMutex& mutex) { CPPUNIT_ASSERT_EQUAL(level, mutex.lockLevel()); }
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
		A a;
		MutexWrapper<A> w(a);
		w->f(1, w.mutex());
		CPPUNIT_ASSERT_EQUAL(0, w.mutex().lockLevel());
	}

	void
	testDirect()
	{
		A a;
		MutexWrapper<A> w(a);
		w.direct().f(0, w.mutex());
		CPPUNIT_ASSERT_EQUAL(0, w.mutex().lockLevel());
	}

	void
	testOwned()
	{
		MutexWrapper<A> w(new A);
		w->f(1, w.mutex());
		w.direct().f(0, w.mutex());
		CPPUNIT_ASSERT_EQUAL(0, w.mutex().lockLevel());
	}
};

} // namespace

CPPUNIT_TEST_SUITE_REGISTRATION(WrapperTester);
