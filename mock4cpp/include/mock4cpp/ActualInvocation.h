#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <typeinfo>
#include <unordered_set>
#include <tuple>

struct Method {
};

struct AnyInvocation {

	AnyInvocation(const int ordinal, const Method & method) :
			ordinal(ordinal), method(method), _isVerified(false) {
	}

	virtual ~AnyInvocation() = default;

	int getOrdinal() {
		return ordinal;
	}

	const Method & getMethod(){
		return method;
	}

	void markAsVerified(){
		_isVerified = true;
	}

	bool isVerified(){
		return _isVerified;
	}

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

	const std::tuple<arglist...>& getActualArguments() {
		return actualArguments;
	}

private:
	std::tuple<arglist...> actualArguments;
};

struct ActualInvocationsSource {
	virtual void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const = 0;
};

#endif // ActualInvocation_h__
