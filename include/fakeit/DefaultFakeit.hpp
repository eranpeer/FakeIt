/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultFakeit_h__
#define DefaultFakeit_h__

#include "fakeit/FakeitContext.hpp"
#include "fakeit/DefaultEventHandler.hpp"
#include "fakeit/DefaultErrorFormatter.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/VerifyFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsFunctor.hpp"

namespace fakeit {

struct DefaultFakeit: public FakeitContext {

	DefaultFakeit() :_customFormatter(nullptr){}

	static DefaultFakeit& getInstance() {
		static DefaultFakeit instance;
		return instance;
	}

	void setCustomErrorFormatter(ErrorFormatter& userDefinedFormatter) {
		_customFormatter = &userDefinedFormatter;
	}

	void clearCustomErrorFormatter() {
		_customFormatter = nullptr;
	}

protected:
	
	EventHandler& getEventHandler() override {
		return _eventHandler;
	}

	ErrorFormatter& getErrorFormatter() override {
		if (_customFormatter)
			return *_customFormatter;
		return _formatter;
	}

private:
	DefaultEventHandler _eventHandler;
	DefaultErrorFormatter _formatter;
	ErrorFormatter * _customFormatter;
};

static DefaultFakeit& Fakeit = DefaultFakeit::getInstance();
static UsingFunctor Using(Fakeit);
static VerifyFunctor Verify(Fakeit);
static VerifyNoOtherInvocationsFunctor VerifyNoOtherInvocations(Fakeit);

}
#endif //
