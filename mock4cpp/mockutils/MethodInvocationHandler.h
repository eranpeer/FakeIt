#ifndef MethodInvocationHandler_h__
#define MethodInvocationHandler_h__

template <typename R, typename... arglist>
struct MethodInvocationHandler
{
	virtual ~MethodInvocationHandler() = 0 {};
	virtual R handleMethodInvocation(const arglist&... args) = 0;
};
#endif // MethodInvocationHandler_h__



