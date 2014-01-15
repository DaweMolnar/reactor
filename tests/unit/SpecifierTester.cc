#include <net/Ip.hh>
#include <net/Port.hh>

#include <cppunit/extensions/HelperMacros.h>

using namespace net;

class SpecifierTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SpecifierTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testSpecifier);
	CPPUNIT_TEST(testHost);
	CPPUNIT_TEST(testIp);
	CPPUNIT_TEST(testService);
	CPPUNIT_TEST(testPort);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		Specifier spec;
		Host host("localhost");
		Ip ip("127.0.0.1");
		Service serv("http");
		Port port("80");
	}

	void
	testSpecifier()
	{
		Specifier specOrig, specCopy(specOrig), spec;

		spec = specCopy;
		CPPUNIT_ASSERT_EQUAL(std::string(""), spec.spec());
		CPPUNIT_ASSERT_EQUAL(0, spec.aiFlags());
	}

	void
	testHost()
	{
		Host hostOrig("localhost"), hostCopy(hostOrig), host;

		host = hostCopy;
		CPPUNIT_ASSERT_EQUAL(std::string("localhost"), host.spec());
		CPPUNIT_ASSERT_EQUAL(0, host.aiFlags());
	}

	void
	testIp()
	{
		Ip ip("127.0.0.1");

		CPPUNIT_ASSERT_EQUAL(std::string("127.0.0.1"), ip.spec());
		CPPUNIT_ASSERT_EQUAL(Host::NUMERIC, ip.aiFlags());
	}

	void
	testService()
	{
		Service servOrig("http"), servCopy(servOrig), serv;

		serv = servCopy;
		CPPUNIT_ASSERT_EQUAL(std::string("http"), serv.spec());
		CPPUNIT_ASSERT_EQUAL(0, serv.aiFlags());
	}

	void
	testPort()
	{
		Port port("80");

		CPPUNIT_ASSERT_EQUAL(std::string("80"), port.spec());
		CPPUNIT_ASSERT_EQUAL(Service::NUMERIC, port.aiFlags());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SpecifierTester);
