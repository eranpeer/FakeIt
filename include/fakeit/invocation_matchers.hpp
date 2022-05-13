/*
 * matchers.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Aug 12, 2014
 */
#pragma once

#include <functional>
#include <tuple>
#include <string>
#include <iosfwd>

#include "mockutils/TupleDispatcher.hpp"
#include "mockutils/TuplePrinter.hpp"
#include "mockutils/DefaultValue.hpp"
#include "mockutils/type_utils.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/argument_matchers.hpp"

namespace fakeit {

    template<typename ... arglist>
    struct ArgumentsMatcherInvocationMatcher : public ActualInvocation<arglist...>::Matcher {

        virtual ~ArgumentsMatcherInvocationMatcher() {
            for (unsigned int i = 0; i < _matchers.size(); i++)
                delete _matchers[i];
        }

        ArgumentsMatcherInvocationMatcher(const std::vector<Destructible *> &args)
                : _matchers(args) {
        }

        virtual bool matches(ActualInvocation<arglist...> &invocation) override {
            if (invocation.getActualMatcher() == this)
                return true;
            return matches(invocation.getActualArguments());
        }

        virtual std::string format() const override {
            std::ostringstream out;
            out << "(";
            for (unsigned int i = 0; i < _matchers.size(); i++) {
                if (i > 0) out << ", ";
                IMatcher *m = dynamic_cast<IMatcher *>(_matchers[i]);
                out << m->format();
            }
            out << ")";
            return out.str();
        }

    private:

        struct MatchingLambda {
            MatchingLambda(const std::vector<Destructible *> &matchers)
                    : _matchers(matchers) {
            }

            template<typename A>
            void operator()(int index, A &actualArg) {
                TypedMatcher<typename naked_type<A>::type> *matcher =
                        dynamic_cast<TypedMatcher<typename naked_type<A>::type> *>(_matchers[index]);
                if (_matching)
                    _matching = matcher->matches(actualArg);
            }

            bool isMatching() {
                return _matching;
            }

        private:
            bool _matching = true;
            const std::vector<Destructible *> &_matchers;
        };

        virtual bool matches(ArgumentsTuple<arglist...>& actualArguments) {
            MatchingLambda l(_matchers);
            fakeit::TupleDispatcher::for_each(actualArguments, l);
            return l.isMatching();
        }

        const std::vector<Destructible *> _matchers;
    };

//template<typename ... arglist>
//struct ExpectedArgumentsInvocationMatcher: public ActualInvocation<arglist...>::Matcher {
//
//	virtual ~ExpectedArgumentsInvocationMatcher() = default;
//
//	ExpectedArgumentsInvocationMatcher(const arglist&... args)
//			: expectedArguments(args...) {
//	}
//
//	ExpectedArgumentsInvocationMatcher(const std::tuple<arglist...>& expectedArguments)
//			: expectedArguments(expectedArguments) {
//	}
//
//	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
//		if (invocation.getActualMatcher() == this)
//			return true;
//		return matches(invocation.getActualArguments());
//	}
//
//	virtual std::string format() const override {
//		std::ostringstream out;
//		print(out, expectedArguments);
//		return out.str();
//	}
//private:
//	virtual bool matches(const std::tuple<arglist...>& actualArgs) {
//		return expectedArguments == actualArgs;
//	}
//	const std::tuple<arglist...> expectedArguments;
//};

    template<typename ... arglist>
    struct UserDefinedInvocationMatcher : ActualInvocation<arglist...>::Matcher {
        virtual ~UserDefinedInvocationMatcher() = default;

        UserDefinedInvocationMatcher(const std::function<bool(arglist &...)>& match)
                : matcher{match} {
        }

        virtual bool matches(ActualInvocation<arglist...> &invocation) override {
            if (invocation.getActualMatcher() == this)
                return true;
            return matches(invocation.getActualArguments());
        }

        virtual std::string format() const override {
            return {"( user defined matcher )"};
        }

    private:
        virtual bool matches(ArgumentsTuple<arglist...>& actualArguments) {
            return TupleDispatcher::invoke<bool, typename tuple_arg<arglist>::type...>(matcher, actualArguments);
        }

        const std::function<bool(arglist &...)> matcher;
    };

    template<typename ... arglist>
    struct DefaultInvocationMatcher : public ActualInvocation<arglist...>::Matcher {

        virtual ~DefaultInvocationMatcher() = default;

        DefaultInvocationMatcher() {
        }

        virtual bool matches(ActualInvocation<arglist...> &invocation) override {
            return matches(invocation.getActualArguments());
        }

        virtual std::string format() const override {
            return {"( Any arguments )"};
        }

    private:
        
        virtual bool matches(const ArgumentsTuple<arglist...>&) {
            return true;
        }
    };

}
