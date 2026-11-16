/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "mockutils/Destructible.hpp"
#include "mockutils/type_utils.hpp"
#include "fakeit/ActualInvocationsSource.hpp"

namespace fakeit {

    struct NoMoreRecordedActionException {
    };

    // Non-template base so DynamicProxy can reach any method mock's invocations without dynamic_cast.
    struct MethodInvocationHandlerBase : Destructible, ActualInvocationsSource, ActualInvocationsContainer {
    };

    template<typename R, typename ... arglist>
    struct MethodInvocationHandler : MethodInvocationHandlerBase {
        virtual R handleMethodInvocation(const typename fakeit::production_arg<arglist>::type... args) = 0;
    };

}
