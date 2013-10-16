#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <functional>

template <typename... arglist>
struct ActualInvocation
{
	ActualInvocation(const arglist&... args) : actualArguments{ args... }
	{
	}
	virtual bool matches(const arglist&... actualArgs) {
		return actualArguments == std::tuple<arglist...>{actualArgs...};
	}
private:
	const std::tuple <arglist...> actualArguments;
};
#endif // ActualInvocation_h__
