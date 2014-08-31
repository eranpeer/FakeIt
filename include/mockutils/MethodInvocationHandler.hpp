/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef MethodInvocationHandler_h__
#define MethodInvocationHandler_h__

#include "mockutils/Destructable.hpp"

namespace fakeit {

struct NoMoreRecordedActionException {
};

template<typename R, typename ... arglist>
struct MethodInvocationHandler: public Destructable {
	virtual ~MethodInvocationHandler() = default;
	virtual R handleMethodInvocation(arglist&... args) = 0;
};

}
#endif // MethodInvocationHandler_h__

