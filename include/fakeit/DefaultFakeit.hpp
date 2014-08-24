/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultFakeit_h__
#define DefaultFakeit_h__

#include "fakeit/FakeIt.hpp"
#include "fakeit/DefaultEventHandler.hpp"
#include "fakeit/DefaultErrorFormatter.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/VerifyFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsFunctor.hpp"

namespace fakeit {

struct DefaultFakeit: public FakeIt {

	static DefaultFakeit& getInstance() {
		static DefaultFakeit instance;
		return instance;
	}

	EventHandler& getEventHandler() override {
		return eventHandler;
	}

	ErrorFormatter& getErrorFormatter() override {
		return formatter;
	}

private:
	DefaultEventHandler eventHandler;
	DefaultErrorFormatter formatter;
};

static UsingFunctor Using (DefaultFakeit::getInstance());
static VerifyFunctor Verify(DefaultFakeit::getInstance());
static VerifyNoOtherInvocationsFunctor VerifyNoOtherInvocations(DefaultFakeit::getInstance());

}
#endif //
