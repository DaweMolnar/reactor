#include "MockRegistry.hh"

#include "Mocked.hh"
#include "MockException.hh"

MockRegistry &
MockRegistry::getInstance()
{
	static MockRegistry instance;
	return instance;
}

void
MockRegistry::add(Mocked *mocked)
{
	MockRegistry &mr = getInstance();
	mr.mockeds_.insert(std::make_pair(mocked->name(), mocked));
}

void
MockRegistry::remove(const Mocked *mocked)
{
	MockRegistry &mr = getInstance();
	mr.mockeds_.erase(mocked->name());
}

Mocked &
MockRegistry::find(const std::string &name)
{
	MockRegistry &mr = getInstance();
	Mockeds::iterator i = mr.mockeds_.find(name);
	if (i != mr.mockeds_.end()) {
		return *(i->second);
	} else {
		throw MockException(name, "no such mock");
	}
}
