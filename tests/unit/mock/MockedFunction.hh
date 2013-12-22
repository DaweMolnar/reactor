#ifndef REACTOR_MOCKED_FUNCTION_HEADER
#define REACTOR_MOCKED_FUNCTION_HEADER

#include <tests/unit/mock/Mocked.hh>

#include <memory> // std::auto_ptr

template <typename F>
class MockedFunction : public Mocked {
	F *wrap_;
	F orig_;

public:
	MockedFunction(const std::string &name, F *wrap, F mock)
	: Mocked(name)
	, wrap_(wrap)
	, orig_(*wrap_)
	{
		*wrap_ = mock;
	}

	virtual
	~MockedFunction()
	{
		*wrap_ = orig_;
	}
};

template <typename F>
MockedFunction<F> *
createMockedFunction(const std::string &name, F *wrap, F mock)
{
	return new MockedFunction<F>(name, wrap, mock);
}

#define MOCK_FUNCTION(name, mock_name) \
	std::auto_ptr<Mocked> name(createMockedFunction(#name, &__wrap_ ## name ## _ptr, mock_name))

#define MOCK_FUNCTION_DEFAULT(name) \
	MOCK_FUNCTION(name, mock_ ## name)

#endif // REACTOR_MOCKED_FUNCTION_HEADER
