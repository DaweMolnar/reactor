#include "time.h"

#include <tests/unit/mock/MockRegistry.hh>
#include <tests/unit/mock/Mocked.hh>

#include </usr/include/cppunit/TestAssert.h>

REDIRECT_MOCK_C_FUNCTION2(gettimeofday, int, struct timeval *, tp, struct timezone *, tzp)

int
mock_gettimeofday(struct timeval *tp, struct timezone *tzp)
{
	Mocked &m = MockRegistry::find("gettimeofday");
	CPPUNIT_ASSERT(tp);
	tp->tv_sec = m.expectedInt();
	tp->tv_usec = m.expectedInt();
	CPPUNIT_ASSERT_EQUAL((struct timezone *)0, tzp);
	return m.expectedInt();
}
