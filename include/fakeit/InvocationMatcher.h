#ifndef InvocationMatcher_h__
#define InvocationMatcher_h__

#include <functional>

#include "fakeit/ActualInvocation.h"
#include "mockutils/Macros.h"

namespace fakeit {

template<typename ... arglist>
struct InvocationMatcher: public virtual Destructable {
	virtual bool matches(ActualInvocation<arglist...>& actualInvocation) = 0;
};

}
#endif // InvocationMatcher_h__
