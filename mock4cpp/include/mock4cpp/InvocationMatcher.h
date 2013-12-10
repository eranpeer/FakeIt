#ifndef InvocationMatcher_h__
#define InvocationMatcher_h__

#include <functional>

#include "mock4cpp/ActualInvocation.h"

namespace mock4cpp {

	template <typename... arglist>
	struct InvocationMatcher:public Destructable
	{
		virtual bool matches(ActualInvocation<arglist...>& actualArgs) = 0;
	};
}
#endif // InvocationMatcher_h__


