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

	template<std::size_t N>
		using NakedArgType = typename naked_type<ArgType<index>>::type;

	template<std::size_t N>
	using ArgMatcherCreatorType = ITypedMatcherCreator<NakedArgType<N>>;

	MatchersCollector(std::vector<Destructable*>& matchers)
			: _matchers(matchers) {
	}

	void CollectMatchers() {
	}

	template<typename Head, typename ...Tail> //
	typename std::enable_if< //
			std::is_base_of<ITypedMatcherCreator<NakedArgType<index>>, Head>::value, void> //
	::type CollectMatchers(const Head& creator, const Tail& ... tail) {

		ITypedMatcher<NakedArgType<index>>* d = creator.createMatcher();
		_matchers.push_back(d);
		MatchersCollector<index + 1, arglist...> c(_matchers);
		c.CollectMatchers(tail...);
	}

	template<typename Head, typename ...Tail>
	typename std::enable_if< //
			!std::is_base_of<ITypedMatcherCreator<NakedArgType<index>>, Head>::value //
			&& !std::is_same<AnyMatcher, Head>::value, void> //
	::type CollectMatchers(const Head& value, const Tail& ... tail) {

		EqMatcherCreator<NakedArgType<index>> m(value);
		ITypedMatcher<NakedArgType<index>>* d = m.createMatcher();
		_matchers.push_back(d);
		MatchersCollector<index + 1, arglist...> c(_matchers);
		c.CollectMatchers(tail...);
	}

	template<typename Head, typename ...Tail>
	typename std::enable_if< //
			!std::is_base_of<ITypedMatcherCreator<NakedArgType<index>>, Head>::value //
			&& std::is_same<AnyMatcher, Head>::value, void> //
	::type CollectMatchers(const Head& value, const Tail& ... tail) {

		TypedAnyMatcher<NakedArgType<index>> m;
		ITypedMatcher<NakedArgType<index>>* d = m.createMatcher();
		_matchers.push_back(d);
		MatchersCollector<index + 1, arglist...> c(_matchers);
		c.CollectMatchers(tail...);
	}

	template<typename Head>
	typename std::enable_if< //
			std::is_base_of<ITypedMatcherCreator<NakedArgType<index>>, Head>::value, void> //
	::type CollectMatchers(const Head& creator) {

		ITypedMatcher<NakedArgType<index>>* d = creator.createMatcher();
		_matchers.push_back(d);
	}

	template<typename Head>
	typename std::enable_if< //
			!std::is_base_of<ITypedMatcherCreator<NakedArgType<index>>, Head>::value //
			&& !std::is_same<AnyMatcher, Head>::value, void> //
	::type CollectMatchers(const Head& value) {

		EqMatcherCreator<NakedArgType<index>> m(value);
		ITypedMatcher<NakedArgType<index>>* d = m.createMatcher();
		_matchers.push_back(d);
	}

	template<typename Head>
	typename std::enable_if<!std::is_base_of<ITypedMatcherCreator<NakedArgType<index>>, Head>::value //
	&& std::is_same<AnyMatcher, Head>::value, void> //
	::type CollectMatchers(const Head& value) {

		TypedAnyMatcher<NakedArgType<index>> m;
		ITypedMatcher<NakedArgType<index>>* d = m.createMatcher();
		_matchers.push_back(d);
	}

};

}

#endif /* MATCHERSCOLLECTOR_HPP_ */
