/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <functional>
#include <type_traits>
#include <tuple>
#include <memory>
#include <vector>
#include <unordered_set>

#include "mockutils/Destructible.hpp"
#include "mockutils/type_utils.hpp"
#include "fakeit/argument_matchers.hpp"

namespace fakeit {

    template<unsigned int index, typename ... arglist>
    class MatchersCollector {

        std::vector<Destructible *> &_matchers;

    public:

        // Fetch the Nth type from arglist...
        template<std::size_t N>
        using ArgType = typename std::tuple_element<N, std::tuple<arglist...>>::type;

        template<std::size_t N>
        using NakedArgType = typename naked_type<ArgType<index>>::type;

        template <typename MatcherCreatorT, typename = void>
        struct IsMatcherCreatorTypeCompatible : std::false_type {};

        template <typename MatcherCreatorT>
        struct IsMatcherCreatorTypeCompatible<MatcherCreatorT, typename std::enable_if<MatcherCreatorT::template IsTypeCompatible<NakedArgType<index>>::value, void>::type> : std::true_type {};

        MatchersCollector(std::vector<Destructible *> &matchers)
                : _matchers(matchers) {
        }

        void CollectMatchers() {
        }

        template<typename Head>
        typename std::enable_if< //
                !std::is_same<AnyMatcher, typename naked_type<Head>::type>::value &&
                !IsMatcherCreatorTypeCompatible<typename naked_type<Head>::type>::value &&
                std::is_constructible<NakedArgType<index>, Head&&>::value, void> //
        ::type CollectMatchers(Head &&value) {

            TypedMatcher<NakedArgType<index>> *d = Eq(std::forward<Head>(value)).template createMatcher<NakedArgType<index>>();
            _matchers.push_back(d);
        }

        template<typename Head>
        typename std::enable_if< //
                IsMatcherCreatorTypeCompatible<typename naked_type<Head>::type>::value, void> //
        ::type CollectMatchers(Head &&creator) {
            TypedMatcher<NakedArgType<index>> *d = creator.template createMatcher<NakedArgType<index>>();
            _matchers.push_back(d);
        }

        template<typename Head>
        typename std::enable_if<//
                std::is_same<AnyMatcher, typename naked_type<Head>::type>::value, void> //
        ::type CollectMatchers(Head &&) {
            TypedMatcher<NakedArgType<index>> *d = Any().template createMatcher<NakedArgType<index>>();
            _matchers.push_back(d);
        }

        template<typename Head, typename ...Tail>
        void CollectMatchers(Head &&head, Tail &&... tail) {
            CollectMatchers(std::forward<Head>(head));
            MatchersCollector<index + 1, arglist...> c(_matchers);
            c.CollectMatchers(std::forward<Tail>(tail)...);
        }

    };

}
