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
#include "fakeit/SpyFunctor.hpp"
#include "fakeit/FakeFunctor.hpp"
#include "fakeit/WhenFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsFunctor.hpp"

namespace fakeit {

class NullEventHandler : public fakeit::EventHandler {

	virtual void handle(const UnexpectedMethodCallEvent&) {}

	virtual void handle(const SequenceVerificationEvent&) {}

	virtual void handle(const NoMoreInvocationsVerificationEvent&) {}

	NullEventHandler(NullEventHandler&) = delete;

public:
	NullEventHandler() = default;
};

class DefaultFakeit : public FakeitContext
{

public:
	DefaultFakeit():_formatter(),_eventLogger(*this), _customFormatter(nullptr),_testingFrameworkAdapter(nullptr)
	{
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

	fakeit::EventHandler& getTestingFrameworkAdapter()  {
		if (_testingFrameworkAdapter)
			return *_testingFrameworkAdapter;
		return _nullTestingFrameworkAdapter;
	}

	fakeit::EventFormatter& getEventFormatter()  {
		if (_customFormatter)
			return *_customFormatter;
		return _formatter;
	}

private:

	DefaultEventFormatter _formatter;
	DefaultEventLogger _eventLogger;
	NullEventHandler _nullTestingFrameworkAdapter;

	fakeit::EventFormatter * _customFormatter;
	fakeit::EventHandler* _testingFrameworkAdapter;
};

static DefaultFakeit& Fakeit = DefaultFakeit::getInstance();
static UsingFunctor Using(Fakeit);
static VerifyFunctor Verify(Fakeit);
static VerifyNoOtherInvocationsFunctor VerifyNoOtherInvocations(Fakeit);
static SpyFunctor Spy;
static FakeFunctor Fake;
static WhenFunctor When;

template <class T>
class SilenceUnusedVariableWarnings {

	void use(void *){}

	SilenceUnusedVariableWarnings()
	{
		use(&Fake);
		use(&When);
		use(&Spy);
		use(&Using);
		use(&Verify);
		use(&VerifyNoOtherInvocations);
	}
};

}

#endif //
