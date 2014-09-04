/*
 * matchers.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Aug 12, 2014
 */
#ifndef MATCHERS_HPP_
#define MATCHERS_HPP_

#include <functional>
#include <tuple>
#include <string>
#include <iosfwd>

#include "mockutils/TupleDispatcher.hpp"
#include "mockutils/TuplePrinter.hpp"
#include "fakeit/ActualInvocation.hpp"

namespace fakeit {

template<typename ... arglist>
struct ExpectedArgumentsInvocationMatcher: public ActualInvocation<arglist...>::Matcher {

	virtual ~ExpectedArgumentsInvocationMatcher() = default;

	ExpectedArgumentsInvocationMatcher(const arglist&... args) :
			expectedArguments(args...) {
	}

	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
		if (invocation.getActualMatcher() == this)
			return true;
		return matches(invocation.getActualArguments());
	}

	virtual std::string format() const {
		std::ostringstream out;
		print(out,expectedArguments);
		return out.str();
	}
private:
	virtual bool matches(const std::tuple<arglist...>& actualArgs) {
		return expectedArguments == actualArgs;
	}
	const std::tuple<arglist...> expectedArguments;
};

template<typename ... arglist>
struct UserDefinedInvocationMatcher: public ActualInvocation<arglist...>::Matcher {
	virtual ~UserDefinedInvocationMatcher() = default;

	UserDefinedInvocationMatcher(std::function<bool(arglist...)> matcher) :
			matcher { matcher } {
	}

	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
		if (invocation.getActualMatcher() == this)
			return true;
		return matches(invocation.getActualArguments());
	}

	virtual std::string format() const {
		return {"( user defined matcher )"};
	}

private:
	virtual bool matches(const std::tuple<arglist...>& actualArgs) {
		return invoke<arglist...>(matcher, std::tuple<arglist...> { actualArgs });
	}
	std::function<bool(arglist...)> matcher;
};

template<typename ... arglist>
struct DefaultInvocationMatcher: public ActualInvocation<arglist...>::Matcher {

	virtual ~DefaultInvocationMatcher() = default;

	DefaultInvocationMatcher() {
	}

	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
		return matches(invocation.getActualArguments());
	}

	virtual std::string format() const {
		return {"( any arguments )"};
	}

private:
	virtual bool matches(const std::tuple<arglist...>&) {
		return true;
	}
};


}

#endif /* MATCHERS_HPP_ */
