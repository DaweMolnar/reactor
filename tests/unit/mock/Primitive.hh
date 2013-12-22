#ifndef REACTOR_PRIMITIVE_HEADER
#define REACTOR_PRIMITIVE_HEADER

template <typename T> class Primitive;

typedef Primitive<int> Integer;
typedef Primitive<void *> Pointer;

class PrimitiveVisitor {
public:
	virtual ~PrimitiveVisitor() {}

	virtual void visit(Integer &) = 0;
	virtual void visit(Pointer &) = 0;
};

class PrimitiveBase {
public:
	virtual ~PrimitiveBase() {}

	virtual void accept(PrimitiveVisitor &visitor) = 0;
};

template <typename T>
class Primitive : public PrimitiveBase {
	T value_;

public:
	explicit Primitive(const T &value) : value_(value) {}

	const T &value() const { return value_; }
	virtual void accept(PrimitiveVisitor &visitor) { visitor.visit(*this); }
};

#endif // REACTOR_PRIMITIVE_HEADER
