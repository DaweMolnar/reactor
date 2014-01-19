#ifndef REACTOR_TESTS_FUNC_FUNCTIONALTEST_HEADER
#define REACTOR_TESTS_FUNC_FUNCTIONALTEST_HEADER

#include <util/Noncopyable.hh>

#include <map>
#include <string>

struct FunctionalTest {
	virtual ~FunctionalTest() {}

	virtual int run() = 0;
};

template <class T>
FunctionalTest *
createFunctionalTest(int argc, char *argv[])
{
	return new T(argc, argv);
}

class FunctionalTestRegistry : public util::Noncopyable {
	typedef FunctionalTest *(*CreateFunc)(int, char *[]);
	struct Item {
		CreateFunc createFunc;

		Item(CreateFunc createFunc0)
		: createFunc(createFunc0)
		{}
	};
	typedef std::map<std::string, Item> Items;

	Items items_;

	FunctionalTestRegistry() {}

public:
	static FunctionalTestRegistry &
	getInstance()
	{
		static FunctionalTestRegistry instance;

		return instance;
	}

	template <class T>
	void
	add(const std::string &name)
	{
		Item item(createFunctionalTest<T>);
		items_.insert(std::make_pair(name, item));
	}

	FunctionalTest *
	create(const std::string &name, int argc, char *argv[])
	{
		Items::const_iterator i(items_.find(name));
		if (i != items_.end()) {
			return i->second.createFunc(argc, argv);
		} else {
			return 0;
		};
	}
};

template <class T>
class FunctionalTestRegistrator : public util::Noncopyable {
public:
	FunctionalTestRegistrator(const std::string &name)
	{
		FunctionalTestRegistry &reg = FunctionalTestRegistry::getInstance();
		reg.add<T>(name);
	}
};

#define CONCATE(a, b) CONCATE2(a, b)
#define CONCATE2(a, b) a ## b

#define GENERATE_UNIQUE_NAME(base) CONCATE(base, __LINE__)

#define REGISTER_FUNCTIONAL_TEST(type) \
	static FunctionalTestRegistrator<type> GENERATE_UNIQUE_NAME(registrator) ( #type )

#endif // REACTOR_TESTS_FUNC_FUNCTIONALTEST_HEADER
