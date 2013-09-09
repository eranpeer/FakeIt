#ifndef MethodProxy_h__
#define MethodProxy_h__

template <typename R, typename... arglist>
struct MethodProxy {
	virtual unsigned int getOffset() = 0;
	virtual void * getProxy() = 0;
};

template <typename R, typename... arglist>
struct MethodInvocationHandler
{
	virtual ~MethodInvocationHandler() = 0 {};
	virtual R handleMethodInvocation(const arglist&... args) = 0;
};

#endif // MethodProxy_h__