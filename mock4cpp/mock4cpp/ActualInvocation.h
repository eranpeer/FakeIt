#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <typeinfo>

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
	std::tuple<arglist...> actualArguments;
};
#endif // ActualInvocation_h__
