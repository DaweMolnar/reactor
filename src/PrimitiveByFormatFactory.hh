#ifndef REACTOR_PRIMITIVE_BY_FORMAT_FACTORY_HEADER
#define REACTOR_PRIMITIVE_BY_FORMAT_FACTORY_HEADER

#include <src/Primitive.hh>

#include <string>
#include <cstdarg>

class PrimitiveByFormatFactory {
public:
	static PrimitiveBase *create(const std::string &conversionSpecification, va_list ap);
};

#endif // REACTOR_PRIMITIVE_BY_FORMAT_FACTORY_HEADER
