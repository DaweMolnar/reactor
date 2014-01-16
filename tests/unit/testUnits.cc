#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include <cstdlib>

int
main()
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry =
		CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::CompilerOutputter *outputter =
		new CppUnit::CompilerOutputter(&runner.result(), std::cerr);

	runner.addTest(registry.makeTest());
	outputter->setLocationFormat("%p:%l:");
	runner.setOutputter(outputter);
	return runner.run("", false) ? EXIT_SUCCESS : EXIT_FAILURE;
}
