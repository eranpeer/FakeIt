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

namespace fakeit {

struct UnexpectedMethodCallException;
struct SequenceVerificationException;
struct NoMoreInvocationsVerificationException;

struct ErrorFormatter {
	virtual ~ErrorFormatter() = default;

	virtual std::string format(const fakeit::UnexpectedMethodCallException& e) const = 0;

	virtual std::string format(const fakeit::SequenceVerificationException& e) const = 0;

	virtual std::string format(const fakeit::NoMoreInvocationsVerificationException& e) const = 0;

};

//static ErrorFormatter* _errorFormatter = nullptr;

//static void setErrorFormatter(ErrorFormatter * errorFormatter){
//	_errorFormatter = errorFormatter;
//}

//static ErrorFormatter* getErrorFormatter() {
//	return _errorFormatter;
//}

}
#endif
