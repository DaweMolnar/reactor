#include "FunctionalTest.hh"

#include <memory>

int
main(int argc, char *argv[])
{
	FunctionalTestRegistry &reg = FunctionalTestRegistry::getInstance();
	std::auto_ptr<FunctionalTest> test(reg.create("ClientTester", argc, argv));
	return test->run();
}
