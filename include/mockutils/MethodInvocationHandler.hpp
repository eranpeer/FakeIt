#ifndef MethodInvocationHandler_h__
#define MethodInvocationHandler_h__

#include "mockutils/Destructable.hpp"

namespace fakeit {

template<typename R, typename ... arglist>
struct MethodInvocationHandler: public Destructable {
	virtual ~MethodInvocationHandler() = default;
	virtual R handleMethodInvocation(arglist&... args) = 0;
};

}
#endif // MethodInvocationHandler_h__

