#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <functional>

template <typename... arglist>
struct ActualInvocation
{
	ActualInvocation(const arglist&... args) : actualArguments{ args... }
	{
	}

	const std::tuple <arglist...>& getActualArguments(){
		return actualArguments;
	}

private:
	const std::tuple <arglist...> actualArguments;
};
#endif // ActualInvocation_h__
