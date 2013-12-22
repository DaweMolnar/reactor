#include "PrimitiveByFormatFactory.hh"

#include <stdexcept>

PrimitiveBase *
PrimitiveByFormatFactory::create(const std::string &conversionSpecification, va_list ap)
{
	if (conversionSpecification == "d") {
		return new Integer(va_arg(ap, int));
	} else if (conversionSpecification == "p") {
		return new Pointer(va_arg(ap, void *));
	} else {
		throw std::runtime_error("unknown conversion specification (" + conversionSpecification + ")");
	}
}
