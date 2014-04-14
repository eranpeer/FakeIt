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
#include <sstream>

//#include "mockutils/TuplePrinter.hpp"
#include "mockutils/Macros.hpp"

#include "fakeit/Invocation.hpp"

namespace fakeit {

template<typename ... arglist>
struct ActualInvocation: public virtual Invocation {

	ActualInvocation(const int ordinal, const Method & method, const arglist&... args) :

			Invocation(ordinal, method), actualArguments { args... } {
	}

	const std::tuple<arglist...>& getActualArguments() const {
		return actualArguments;
	}

//	virtual std::string format() override {
//		std::stringstream strm;
//		strm << getMethod().getMethodName();
//		strm << getActualArguments();
//		return strm.str();
//	}

	struct Matcher: public virtual Destructable {
		virtual bool matches(ActualInvocation<arglist...>& actualInvocation) = 0;
	};

	/**
	 * The Matcher that was use to match this ActualInvocation.
	 */
	void setActualMatcher(std::shared_ptr<Matcher> matcher){
		this->matcher = matcher;
	}

	std::shared_ptr<Matcher> getActualMatcher(){
		return matcher;
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
