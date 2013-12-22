#ifndef REACTOR_REDIRECT_MOCK_C_FUNCTION_HEADER
#define REACTOR_REDIRECT_MOCK_C_FUNCTION_HEADER

#ifdef __cplusplus
#define REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN extern "C" {
#define REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END } // extern "C"
#else
#define REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN
#define REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END
#endif

#define REDIRECT_MOCK_C_FUNCTION1(name, ret, t1, n1) \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN \
		ret __real_ ## name (t1); \
		ret (*__wrap_ ## name ## _ptr)(t1) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1) { return __wrap_ ## name ## _ptr(n1); } \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END

#define REDIRECT_MOCK_C_FUNCTION2(name, ret, t1, n1, t2, n2) \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN \
		ret __real_ ## name (t1, t2); \
		ret (*__wrap_ ## name ## _ptr)(t1, t2) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1, t2 n2) { return __wrap_ ## name ## _ptr(n1, n2); } \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END

#define REDIRECT_MOCK_C_FUNCTION3(name, ret, t1, n1, t2, n2, t3, n3) \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN \
		ret __real_ ## name (t1, t2, t3); \
		ret (*__wrap_ ## name ## _ptr)(t1, t2, t3) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1, t2 n2, t3 n3) { return __wrap_ ## name ## _ptr(n1, n2, n3); } \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END

#define REDIRECT_MOCK_C_FUNCTION4(name, ret, t1, n1, t2, n2, t3, n3, t4, n4) \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN \
		ret __real_ ## name (t1, t2, t3, t4); \
		ret (*__wrap_ ## name ## _ptr)(t1, t2, t3, t4) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1, t2 n2, t3 n3, t4 n4) { return __wrap_ ## name ## _ptr(n1, n2, n3, n4); } \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END

#define REDIRECT_MOCK_C_FUNCTION5(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_BEGIN \
		ret __real_ ## name (t1, t2, t3, t4, t5); \
		ret (*__wrap_ ## name ## _ptr)(t1, t2, t3, t4, t5) = __real_ ## name; \
		ret __wrap_ ## name (t1 n1, t2 n2, t3 n3, t4 n4, t5 n5) { return __wrap_ ## name ## _ptr(n1, n2, n3, n4, n5); } \
	REDIRECT_MOCK_C_FUNCTION_EXTERN_C_END

#endif // REACTOR_REDIRECT_MOCK_C_FUNCTION_HEADER
