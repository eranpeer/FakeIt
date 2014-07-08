/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultLogger_h__
#define DefaultLogger_h__

#include "fakeit/Logger.hpp"

namespace fakeit {

struct DefaultLogger: public fakeit::Logger {

	virtual void log(UnexpectedMethodCallException& e) override {
	}

	virtual void log(SequenceVerificationException& e) override {
	}

	virtual void log(NoMoreInvocationsVerificationException& e) override {
	}

};
}

#endif //Logger_h__
