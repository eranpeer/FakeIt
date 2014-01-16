#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <typeinfo>

struct Method {
};

struct ActualInvocationInfo {

	ActualInvocationInfo(const int ordinal, const Method & method) :
			ordinal(ordinal), method(method) {
	}

	int getOrdinal() {
		return ordinal;
	}

private:
	const int ordinal;
	const Method & method;
};

template<typename ... arglist>
struct ActualInvocation: public virtual ActualInvocationInfo {

	ActualInvocation(const int ordinal, const Method & method, const arglist&... args) :
			ActualInvocationInfo(ordinal, method), actualArguments { args... } {
	}

	const std::tuple<arglist...>& getActualArguments() {
		return actualArguments;
	}

private:
	std::tuple<arglist...> actualArguments;
};

#endif // ActualInvocation_h__
