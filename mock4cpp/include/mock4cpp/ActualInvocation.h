#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <typeinfo>
#include <unordered_set>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>
#include "mockutils/TuplePrinter.h"
#include "mock4cpp/MockRepository.h"

namespace mock4cpp {


struct AnyInvocation {

	AnyInvocation(const int ordinal, const Method & method) :
			ordinal(ordinal), method(method), _isVerified(false) {
	}

	virtual ~AnyInvocation() = default;

	int getOrdinal() const {
		return ordinal;
	}

	const Method & getMethod() const {
		return method;
	}

	void markAsVerified() {
		_isVerified = true;
	}

	bool isVerified() const {
		return _isVerified;
	}

	virtual std::string format() = 0;

private:
	const int ordinal;
	const Method & method;
	bool _isVerified;
};

template<typename ... arglist>
struct ActualInvocation: public virtual AnyInvocation {

	ActualInvocation(const int ordinal, const Method & method, const arglist&... args) :

	AnyInvocation(ordinal, method), actualArguments { args... } {
	}

	const std::tuple<arglist...>& getActualArguments() const {
		return actualArguments;
	}

	virtual std::string format() override {
		std::stringstream strm;
		strm << getMethod().getMethodName();
		strm << getActualArguments();
		return strm.str();
	}

private:
	std::tuple<arglist...> actualArguments;
};

template<typename ... arglist>
std::ostream & operator<<(std::ostream &strm, const ActualInvocation<arglist...>& ai) {
	strm << ai.format();
	return strm;
}

struct ActualInvocationsSource {
	virtual void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const = 0;
};

}
#endif // ActualInvocation_h__
