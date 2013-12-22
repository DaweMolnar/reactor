#ifndef REACTOR_MOCK_REGISTRY_HEADER
#define REACTOR_MOCK_REGISTRY_HEADER

#include "Noncopyable.hh"

#include <map>
#include <string>

class Mocked;

class MockRegistry : public Noncopyable {
	typedef std::map<std::string, Mocked *> Mockeds;

	Mockeds mockeds_;

	MockRegistry() {}

	static MockRegistry &getInstance();

public:
	static void add(Mocked *mocked);
	static void remove(const Mocked *mocked);
	static Mocked &find(const std::string &name);
};

#endif // REACTOR_MOCK_REGISTRY_HEADER
