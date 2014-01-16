#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <typeinfo>

struct Method {
};

struct AnyInvocation {

	AnyInvocation(const int ordinal, const Method & method) :
			ordinal(ordinal), method(method) {
	}

	virtual ~AnyInvocation() = default;

	int getOrdinal() {
		return ordinal;
	}

	const Method & getMethod(){
		return method;
	}

private:
	const int ordinal;
	const Method & method;
};

template<typename ... arglist>
struct ActualInvocation: public virtual AnyInvocation {

	ActualInvocation(const int ordinal, const Method & method, const arglist&... args) :
		AnyInvocation(ordinal, method), actualArguments { args... } {
	}

	const std::tuple<arglist...>& getActualArguments() {
		return actualArguments;
	}

private:
	std::tuple<arglist...> actualArguments;
};

#endif // ActualInvocation_h__
