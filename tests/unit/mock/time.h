#ifndef TESTS_UNIT_MOCK_UNISTD_HEADER
#define TESTS_UNIT_MOCK_UNISTD_HEADER

#include <tests/unit/mock/RedirectMockCFunction.h>

#include <time.h>

DECLARE_MOCK_C_FUNCTION2(gettimeofday, int, struct timeval *, struct timezone *)
int mock_gettimeofday(struct timeval *tp, struct timezone *tzp);

#endif // TESTS_UNIT_MOCK_UNISTD_HEADER
