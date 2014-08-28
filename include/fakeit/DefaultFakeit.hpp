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

	DefaultFakeit() : _eventLogger(*this), _customFormatter(nullptr),_testingFrameworkAdapter(nullptr) {
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

	void setTestingFrameworkAdapter(fakeit::EventHandler& eventHandler) {
		_testingFrameworkAdapter = &eventHandler;
	}

	void clearTestingFrameworkAdapter() {
		_testingFrameworkAdapter = nullptr;
	}

protected:
	
	fakeit::EventHandler& getTestingFrameworkAdapter() override {
		if (_testingFrameworkAdapter)
			return *_testingFrameworkAdapter;
		return _nullTestingFrameworkAdapter;
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
	NullEventHandler _nullTestingFrameworkAdapter;
	fakeit::EventHandler* _testingFrameworkAdapter;
};

static DefaultFakeit& Fakeit = DefaultFakeit::getInstance();
static UsingFunctor Using(Fakeit);
static VerifyFunctor Verify(Fakeit);
static VerifyNoOtherInvocationsFunctor VerifyNoOtherInvocations(Fakeit);

}
#endif //
