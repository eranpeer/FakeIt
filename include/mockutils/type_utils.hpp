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
#include <tuple>


namespace fakeit {

    template<class C>
    struct naked_type {
        typedef typename std::remove_cv<typename std::remove_reference<C>::type>::type type;
    };

    template< class T > struct left_type         { typedef T type; };
    template< class T > struct left_type < T& > { typedef T& type; };
    template< class T > struct left_type < T&& > { typedef T type; };

    template<typename... arglist>
    using ArgumentsTuple = std::tuple<typename left_type<arglist>::type...>;


    template< class T > struct arg_type        { typedef T& type; };
    template< class T > struct arg_type < T& > { typedef T& type; };
    template< class T > struct arg_type < T&& > { typedef T&& type; };


    template<typename R, typename... arglist>
    struct VTableMethodType {
#if defined (__GNUG__)
        typedef R(*type)(void *, arglist...);
#elif defined (_MSC_VER)
        typedef R(__thiscall *type)(void *, arglist...);
#endif
    };


}
