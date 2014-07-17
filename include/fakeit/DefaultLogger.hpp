/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultLogger_h__
#define DefaultLogger_h__

#include <iostream>
#include "fakeit/Logger.hpp"

namespace fakeit {

struct DefaultLogger: public fakeit::Logger {

	virtual void log(UnexpectedMethodCallException& e) override {
		out << e << std::endl;
	}

	virtual void log(SequenceVerificationException& e) override {
//		for (unsigned int i = 0;i<e.expectedPattern().size();i++){
//			Sequence* s = e.expectedPattern()[i];
//		}

		for (unsigned int i = 0;i<e.actualSequence().size();i++){
			const Method& m = e.actualSequence()[i]->getMethod();
			out << m.name() << std::endl;
		}
		out << e << std::endl;
	}

	virtual void log(NoMoreInvocationsVerificationException& e) override {
		out << e << std::endl;
	}

	DefaultLogger() :
			out(std::cout) {
	}
private:
	std::ostream& out;
};
}

#endif //Logger_h__
