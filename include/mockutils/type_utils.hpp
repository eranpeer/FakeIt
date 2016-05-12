/*
 * type_utils.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jan 14, 2015
 */
#pragma once

#include <tuple>


namespace fakeit {

    template<class C>
    struct naked_type {
        typedef typename std::remove_cv<typename std::remove_reference<C>::type>::type type;
    };

    template< class T > struct tuple_arg         { typedef T  type; };
    template< class T > struct tuple_arg < T& >  { typedef T& type; };
    template< class T > struct tuple_arg < T&& > { typedef T&&  type; };

    //template<typename... arglist>
    //using ArgumentsTuple = std::tuple<typename tuple_arg<arglist>::type...>;

    template<typename... arglist>
    using ArgumentsTuple = std::tuple < arglist... > ;

    template< class T > struct test_arg         { typedef T& type; };
    template< class T > struct test_arg< T& >   { typedef T& type; };
    template< class T > struct test_arg< T&& >  { typedef T& type; };

    template< class T > struct production_arg         { typedef T& type; };
    template< class T > struct production_arg< T& >   { typedef T& type; };
    template< class T > struct production_arg< T&& >  { typedef T&&  type; };

    template <typename T>
    class is_ostreamable {
        struct no {};
#if defined(_MSC_VER) && _MSC_VER < 1900
        template <typename T1>
        static decltype(operator<<(std::declval<std::ostream&>(), std::declval<const T1>())) test(std::ostream &s, const T1 &t);
#else
        template <typename T1>
        static auto test(std::ostream &s, const T1 &t) -> decltype(s << t);
#endif
        static no test(...);
    public:
        // capture std::ostream operator<< members for the benefit of MSVC
        static const bool value =
            std::is_arithmetic<T>::value ||
            std::is_pointer<T>::value ||
            std::is_same<decltype(test(*(std::ostream *)nullptr,
                std::declval<T>())), std::ostream &>::value;
    };

    // capture std::ostream operator<< members for the benefit of MSVC
    template <>
    class is_ostreamable<std::ios_base& (*)(std::ios_base&)> {
    public:
        static const bool value = true;
    };

    template <typename CharT, typename Traits>
    class is_ostreamable<std::basic_ios<CharT,Traits>& (*)(std::basic_ios<CharT,Traits>&)> {
    public:
        static const bool value = true;
    };

    template <typename CharT, typename Traits>
    class is_ostreamable<std::basic_ostream<CharT,Traits>& (*)(std::basic_ostream<CharT,Traits>&)> {
    public:
        static const bool value = true;
    };

    template<typename R, typename... arglist>
    struct VTableMethodType {
#if defined (__GNUG__)
        typedef R(*type)(void *, arglist...);
#elif defined (_MSC_VER)
        typedef R(__thiscall *type)(void *, arglist...);
#endif
    };
}
