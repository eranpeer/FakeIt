/*
 * SpyingContext.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 20, 2014
 */
#pragma once

#include "mockutils/type_utils.hpp"
#include "fakeit/Xaction.hpp"

namespace fakeit {

// For use in Spy(...) phrases
    template<typename R, typename ... arglist>
    struct SpyingContext : Xaction {
        virtual void appendAction(Action<R, arglist...> *action) = 0;

        virtual std::function<R(arglist&...)> getOriginalMethodCopyArgs() = 0;
        virtual std::function<R(arglist&...)> getOriginalMethodForwardArgs() = 0;
    };
}
