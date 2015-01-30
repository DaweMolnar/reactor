#include "FunctionalTest.hh"

#include <util/Noncopyable.hh>

#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

class UsageException
: public std::runtime_error
{
public:
	UsageException(const std::string &what)
	: std::runtime_error(what)
	{}
};

class Main
: public util::Noncopyable
{
	std::unique_ptr<FunctionalTest> test_;

	Main(int argc, char *argv[])
	{
		if (argc < 2) {
			throw UsageException("too few arguments");
		}

		FunctionalTestRegistry &reg = FunctionalTestRegistry::getInstance();
		test_.reset(reg.create(argv[1], argc - 1, argv + 1));
		if (!test_.get()) {
			throw UsageException(std::string("invalid test: ") + argv[1]);
		}
	}

	friend int main(int, char *[]);

	static void
	printTestNames(std::ostream &os)
	{
		typedef FunctionalTestRegistry::TestNames TestNames;

		FunctionalTestRegistry &reg = FunctionalTestRegistry::getInstance();
		TestNames t = reg.getTestNames();

		os << "Available tests:" << std::endl;
		for (TestNames::const_iterator i(t.begin()); i != t.end(); ++i) {
			os << "  " << *i << std::endl;
		}
	}

public:
	static void
	printUsage(std::ostream &os)
	{
		os << "Reactor functional test utility. Usage:" << std::endl;
		os << "  testFuncs [name of test] <args of test> ..." << std::endl;
		os << std::endl;
		printTestNames(os);
		os << std::endl;
	}

	int
	run()
	{
		return test_->run();
	}
};

int
main(int argc, char *argv[])
{
	try {
		return Main(argc, argv).run();
	} catch (const UsageException &e) {
		std::cerr << e.what() << std::endl;
		Main::printUsage(std::cerr);
	}
	return EXIT_FAILURE;
}
