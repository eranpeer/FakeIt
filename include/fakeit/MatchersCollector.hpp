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

        template<std::size_t N>
        using ArgMatcherCreatorType = decltype(std::declval<TypedMatcherCreator<NakedArgType<N>>>());

        MatchersCollector(std::vector<Destructible *> &matchers)
                : _matchers(matchers) {
        }

        void CollectMatchers() {
        }

        template<typename Head>
        typename std::enable_if< //
                std::is_constructible<NakedArgType<index>, Head>::value, void> //
        ::type CollectMatchers(const Head &value) {

            TypedMatcher<NakedArgType<index>> *d = Eq<NakedArgType<index>>(value).createMatcher();
            _matchers.push_back(d);
        }

        template<typename Head, typename ...Tail>
        typename std::enable_if< //
                std::is_constructible<NakedArgType<index>, Head>::value //
                , void> //
        ::type CollectMatchers(const Head &head, const Tail &... tail) {
            CollectMatchers(head);
            MatchersCollector<index + 1, arglist...> c(_matchers);
            c.CollectMatchers(tail...);
        }

        template<typename Head>
        typename std::enable_if< //
                std::is_base_of<TypedMatcherCreator<NakedArgType<index>>, Head>::value, void> //
        ::type CollectMatchers(const Head &creator) {
            TypedMatcher<NakedArgType<index>> *d = creator.createMatcher();
            _matchers.push_back(d);
        }

        template<typename Head, typename ...Tail>
        //
        typename std::enable_if< //
                std::is_base_of<TypedMatcherCreator<NakedArgType<index>>, Head>::value, void> //
        ::type CollectMatchers(const Head &head, const Tail &... tail) {
            CollectMatchers(head);
            MatchersCollector<index + 1, arglist...> c(_matchers);
            c.CollectMatchers(tail...);
        }

        template<typename Head>
        typename std::enable_if<//
                std::is_same<AnyMatcher, Head>::value, void> //
        ::type CollectMatchers(const Head &) {
            TypedMatcher<NakedArgType<index>> *d = Any<NakedArgType<index>>().createMatcher();
            _matchers.push_back(d);
        }

        template<typename Head, typename ...Tail>
        typename std::enable_if< //
                std::is_same<AnyMatcher, Head>::value, void> //
        ::type CollectMatchers(const Head &head, const Tail &... tail) {
            CollectMatchers(head);
            MatchersCollector<index + 1, arglist...> c(_matchers);
            c.CollectMatchers(tail...);
        }

    };

}
