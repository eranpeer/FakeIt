/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef MATCHERSCOLLECTOR_HPP_
#define MATCHERSCOLLECTOR_HPP_

#include <functional>
#include <type_traits>
#include <tuple>
#include <memory>
#include <vector>
#include <unordered_set>

#include "mockutils/type_utils.hpp"

namespace fakeit {

template<unsigned int index, typename ... arglist>
class MatchersCollector {

	std::vector<Destructable*>& _matchers;

public:

	// Fetch the Nth type from arglist...
	template<std::size_t N>
	using ArgType = typename std::tuple_element<N, std::tuple<arglist...>>::type;

	MatchersCollector(std::vector<Destructable*>& matchers)
			: _matchers(matchers) {
	}

	void CollectMatchers() {
	}

	template<typename Head, typename ...Tail>
	typename std::enable_if<std::is_base_of<ITypedMatcherCreator<typename naked_type<ArgType<index>>::type>, Head>::value, void>::type CollectMatchers(
			const Head& creator, const Tail& ... tail) {

		ITypedMatcher<

		typename naked_type<ArgType<index>>::type

		>* d = creator.createMatcher();

		_matchers.push_back(d);
		MatchersCollector<index + 1, arglist...> c(_matchers);
		c.CollectMatchers(tail...);
	}

	template<typename Head, typename ...Tail>
	typename std::enable_if<
			!std::is_base_of<ITypedMatcherCreator<typename naked_type<ArgType<index>>::type>, Head>::value
					&& !std::is_same<AnyMatcher, Head>::value, void>::type CollectMatchers(const Head& value, const Tail& ... tail) {

		EqMatcherCreator<typename naked_type<ArgType<index>>::type> m(value);

		ITypedMatcher<typename naked_type<ArgType<index>>::type>* d = m.createMatcher();
		_matchers.push_back(d);

		MatchersCollector<index + 1, arglist...> c(_matchers);
		c.CollectMatchers(tail...);
	}

	template<typename Head, typename ...Tail>
	typename std::enable_if<
			!std::is_base_of<ITypedMatcherCreator<typename naked_type<ArgType<index>>::type>, Head>::value
					&& std::is_same<AnyMatcher, Head>::value, void>::type CollectMatchers(const Head& value, const Tail& ... tail) {

		TypedAnyMatcher<typename naked_type<ArgType<index>>::type> m;
		ITypedMatcher<typename naked_type<ArgType<index>>::type>* d = m.createMatcher();
		_matchers.push_back(d);

		MatchersCollector<index + 1, arglist...> c(_matchers);
		c.CollectMatchers(tail...);
	}

	template<typename Head>
	typename std::enable_if<std::is_base_of<ITypedMatcherCreator<typename naked_type<ArgType<index>>::type>, Head>::value, void>::type CollectMatchers(
			const Head& creator) {

		ITypedMatcher<

		typename naked_type<ArgType<index>>::type

		>* d = creator.createMatcher();

		_matchers.push_back(d);
	}

	template<typename Head>
	typename std::enable_if<
			!std::is_base_of<ITypedMatcherCreator<typename naked_type<ArgType<index>>::type>, Head>::value
					&& !std::is_same<AnyMatcher, Head>::value, void>::type CollectMatchers(const Head& value) {

		EqMatcherCreator<typename naked_type<ArgType<index>>::type> m(value);

		ITypedMatcher<typename naked_type<ArgType<index>>::type>* d = m.createMatcher();
		_matchers.push_back(d);

	}

	template<typename Head>
	typename std::enable_if<
			!std::is_base_of<ITypedMatcherCreator<typename naked_type<ArgType<index>>::type>, Head>::value
					&& std::is_same<AnyMatcher, Head>::value, void>::type CollectMatchers(const Head& value) {

		TypedAnyMatcher<typename naked_type<ArgType<index>>::type> m;
		ITypedMatcher<typename naked_type<ArgType<index>>::type>* d = m.createMatcher();
		_matchers.push_back(d);
	}

};
}

#endif /* MATCHERSCOLLECTOR_HPP_ */
