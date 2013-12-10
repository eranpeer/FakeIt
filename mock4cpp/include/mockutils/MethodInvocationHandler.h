#ifndef MethodInvocationHandler_h__
#define MethodInvocationHandler_h__

#include "mockutils/Destructable.h"

template <typename R, typename... arglist>
struct MethodInvocationHandler : public Destructable
{
	virtual ~MethodInvocationHandler() = 0 {};
	virtual R handleMethodInvocation(const arglist&... args) = 0;
};
#endif // MethodInvocationHandler_h__



