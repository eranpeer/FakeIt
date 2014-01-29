#ifndef InvocationMatcher_h__
#define InvocationMatcher_h__

#include <functional>

#include "mock4cpp/ActualInvocation.h"

namespace mock4cpp {

struct AnyInvocationMatcher {
	//virtual ~AnyInvocationMatcher() = default;
	virtual bool matches(AnyInvocation& invocation) = 0;
};

template<typename ... arglist>
struct InvocationMatcher: public virtual Destructable {
	virtual bool matches(ActualInvocation<arglist...>& actualInvocation) = 0;
};

}
#endif // InvocationMatcher_h__

