/*
* Copyright (c) 2014 Eran Pe'er.
*
* This program is made available under the terms of the MIT License.
*
* Created on August, 2014
*/
#ifndef ErrorFormatter_h__
#define ErrorFormatter_h__

#include <string>
#include "fakeit/FakeitExceptions.hpp"

namespace fakeit {


	struct ErrorFormatter {
		virtual ~ErrorFormatter() = default;
		virtual std::string format(const fakeit::UnexpectedMethodCallException& e) = 0;
		virtual std::string format(const fakeit::SequenceVerificationException& e) = 0;
		virtual std::string format(const fakeit::NoMoreInvocationsVerificationException& e) = 0;
	};
}

#endif
