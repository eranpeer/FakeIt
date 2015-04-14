/*
 * type_utils.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jan 14, 2015
 */
#pragma once

#include <type_traits>
#include <typeinfo>
#include "mockutils/type_utils.hpp"

namespace fakeit {

    template<class C>
    struct naked_type {
        typedef typename std::remove_cv<typename std::remove_reference<C>::type>::type type;
    };
}
