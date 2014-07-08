/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef Logger_h__
#define Logger_h__

#include "fakeit/FakeitExceptions.hpp"

namespace fakeit {

struct Logger {
	virtual ~Logger() = default;

	virtual void log(UnexpectedMethodCallException& e) = 0;

	virtual void log(SequenceVerificationException& e) = 0;

	virtual void log(NoMoreInvocationsVerificationException& e) = 0;

};
}
#endif //Logger_h__
