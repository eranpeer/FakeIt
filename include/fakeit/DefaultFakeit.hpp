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
#include "fakeit/DefaultEventLogger.hpp"
#include "fakeit/DefaultEventFormatter.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/VerifyFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsFunctor.hpp"

namespace fakeit {

struct DefaultFakeit: public FakeitContext {

	DefaultFakeit() : _eventLogger(*this), _customFormatter(nullptr),_customTestingFrameworkEventHandler(nullptr) {
		addEventHandler(_eventLogger);
	}

	static DefaultFakeit& getInstance() {
		static DefaultFakeit instance;
		return instance;
	}

	void setCustomEventFormatter(fakeit::EventFormatter& customErrorFormatter) {
		_customFormatter = &customErrorFormatter;
	}

	void clearCustomEventFormatter() {
		_customFormatter = nullptr;
	}

	void setCustomTestingFrameworkEventHandler(fakeit::EventHandler& eventHandler) {
		_customTestingFrameworkEventHandler = &eventHandler;
	}

	void clearCustomTestingFrameworkEventHandler() {
		_customTestingFrameworkEventHandler = nullptr;
	}

protected:
	
	fakeit::EventHandler& getTestingFrameworkEventHandler() override {
		if (_customTestingFrameworkEventHandler)
			return *_customTestingFrameworkEventHandler;
		return _nullTestingFrameworkEventHandler;
	}

	fakeit::EventFormatter& getEventFormatter() override {
		if (_customFormatter)
			return *_customFormatter;
		return _formatter;
	}

private:
	class NullEventHandler : public fakeit::EventHandler {

		virtual void handle(const UnexpectedMethodCallEvent& e) {}

		virtual void handle(const SequenceVerificationEvent& e) {}

		virtual void handle(const NoMoreInvocationsVerificationEvent& e) {}
	};

	DefaultEventLogger _eventLogger;
	DefaultEventFormatter _formatter;
	fakeit::EventFormatter * _customFormatter;
	NullEventHandler _nullTestingFrameworkEventHandler;
	fakeit::EventHandler* _customTestingFrameworkEventHandler;
};

static DefaultFakeit& Fakeit = DefaultFakeit::getInstance();
static UsingFunctor Using(Fakeit);
static VerifyFunctor Verify(Fakeit);
static VerifyNoOtherInvocationsFunctor VerifyNoOtherInvocations(Fakeit);

}
#endif //
