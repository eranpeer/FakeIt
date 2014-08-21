/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef ActualInvocation_h__
#define ActualInvocation_h__

#include <typeinfo>
#include <unordered_set>
#include <tuple>
#include <string>
#include <iostream>
#include <atomic>


#include "mockutils/Macros.hpp"
#include "fakeit/Invocation.hpp"
#include "mockutils/TuplePrinter.hpp"


namespace fakeit {

static std::atomic_int invocationOrdinal;

static int nextInvocationOrdinal(){
	return ++invocationOrdinal;
}


template<typename ... arglist>
struct ActualInvocation: public virtual Invocation {

	struct Matcher: public virtual Destructable {
		virtual bool matches(ActualInvocation<arglist...>& actualInvocation) = 0;

		virtual std::string format() const = 0;
	};

	ActualInvocation(int ordinal, Method & method, const arglist&... args) :
			Invocation(ordinal, method), actualArguments { args... } {
	}

	const std::tuple<arglist...>& getActualArguments() const {
		return actualArguments;
	}

	/**
	 * The Matcher that was use to match this ActualInvocation.
	 */
	void setActualMatcher(std::shared_ptr<Matcher> matcher){
		this->matcher = matcher;
	}

	std::shared_ptr<Matcher> getActualMatcher(){
		return matcher;
	}

	virtual std::string format() const {
		std::ostringstream out;
		out << getMethod().name();
		print(out,actualArguments);
		return out.str();
	}

private:
	std::shared_ptr<Matcher> matcher;
	std::tuple<arglist...> actualArguments;
};

template<typename ... arglist>
std::ostream & operator<<(std::ostream &strm, const ActualInvocation<arglist...>& ai) {
	strm << ai.format();
	return strm;
}

struct ActualInvocationsSource {
	virtual void getActualInvocations(std::unordered_set<Invocation*>& into) const = 0;
};

}
#endif // ActualInvocation_h__
