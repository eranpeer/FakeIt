/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <tuple>

namespace fakeit {

    template<int N>
    struct apply_func {
        template<typename R, typename ... ArgsF, typename ... ArgsT, typename ... Args>
        static R applyTuple(std::function<R(ArgsF &...)> f, std::tuple<ArgsT...> &t, Args &... args) {
            return apply_func<N - 1>::template applyTuple(f, t, std::get<N - 1>(t), args...);
        }
    };

    template<>
    struct apply_func < 0 > {
        template<typename R, typename ... ArgsF, typename ... ArgsT, typename ... Args>
        static R applyTuple(std::function<R(ArgsF &...)> f, std::tuple<ArgsT...> & /* t */, Args &... args) {
            return f(args...);
        }
    };

    struct TupleDispatcher {

        template<typename R, typename ... ArgsF, typename ... ArgsT>
        static R applyTuple(std::function<R(ArgsF &...)> f, std::tuple<ArgsT...> &t) {
            return apply_func<sizeof...(ArgsT)>::template applyTuple(f, t);
        }

        template<typename R, typename ...arglist>
        static R invoke(std::function<R(arglist &...)> func, const std::tuple<arglist...> &arguments) {
            std::tuple<arglist...> &args = const_cast<std::tuple<arglist...> &>(arguments);
            return applyTuple(func, args);
        }

        template<typename TupleType, typename FunctionType>
        static void for_each(TupleType &&, FunctionType &,
            std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType>::type>::value>) {
        }

        template<std::size_t I, typename TupleType, typename FunctionType, typename = typename std::enable_if<
            I != std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type>
            static void for_each(TupleType &&t, FunctionType &f, std::integral_constant<size_t, I>) {
            f(I, std::get < I >(t));
            for_each(std::forward < TupleType >(t), f, std::integral_constant<size_t, I + 1>());
        }

        template<typename TupleType, typename FunctionType>
        static void for_each(TupleType &&t, FunctionType &f) {
            for_each(std::forward < TupleType >(t), f, std::integral_constant<size_t, 0>());
        }

        template<typename TupleType1, typename TupleType2, typename FunctionType>
        static void for_each(TupleType1 &&, TupleType2 &&, FunctionType &,
            std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType1>::type>::value>) {
        }

        template<std::size_t I, typename TupleType1, typename TupleType2, typename FunctionType, typename = typename std::enable_if<
            I != std::tuple_size<typename std::remove_reference<TupleType1>::type>::value>::type>
            static void for_each(TupleType1 &&t, TupleType2 &&t2, FunctionType &f, std::integral_constant<size_t, I>) {
            f(I, std::get < I >(t), std::get < I >(t2));
            for_each(std::forward < TupleType1 >(t), std::forward < TupleType2 >(t2), f, std::integral_constant<size_t, I + 1>());
        }

        template<typename TupleType1, typename TupleType2, typename FunctionType>
        static void for_each(TupleType1 &&t, TupleType2 &&t2, FunctionType &f) {
            for_each(std::forward < TupleType1 >(t), std::forward < TupleType2 >(t2), f, std::integral_constant<size_t, 0>());
        }
    };
}