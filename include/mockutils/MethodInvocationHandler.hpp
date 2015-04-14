/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "mockutils/Destructible.hpp"

namespace fakeit {

    struct NoMoreRecordedActionException {
    };

    template<typename R, typename ... arglist>
    struct MethodInvocationHandler : public Destructible {
        virtual R handleMethodInvocation(arglist &... args) = 0;
    };

}
