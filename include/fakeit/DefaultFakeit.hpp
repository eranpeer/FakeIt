/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultFakeit_h__
#define DefaultFakeit_h__

#include "fakeit/EventHandler.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/DefaultEventLogger.hpp"
#include "fakeit/DefaultEventFormatter.hpp"

namespace fakeit {

struct StandaloneAdapter: public EventHandler {

	StandaloneAdapter(EventFormatter& formatter)
			: _formatter(formatter) {
	}

	virtual void handle(const UnexpectedMethodCallEvent& evt) override {
		std::string format = _formatter.format(evt);
		UnexpectedMethodCallException ex(format);
		throw ex;
	}

	virtual void handle(const SequenceVerificationEvent& evt) override {
		std::string format (_formatter.format(evt));
		SequenceVerificationException e(format);
		e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
		throw e;

	}

	virtual void handle(const NoMoreInvocationsVerificationEvent& evt) override {
		std::string format = _formatter.format(evt);
		NoMoreInvocationsVerificationException e(format);
		e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
		throw e;
	}

private:
	EventFormatter& _formatter;
};

class DefaultFakeit: public FakeitContext {

public:
	virtual ~DefaultFakeit() = default;

	DefaultFakeit()
			: _formatter(),
			_standaloneAdapter(*this),
			_customFormatter(nullptr),
			_testingFrameworkAdapter(nullptr) {
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

	fakeit::EventHandler& getTestingFrameworkAdapter() {
		if (_testingFrameworkAdapter)
			return *_testingFrameworkAdapter;
		return _standaloneAdapter;
	}

	fakeit::EventFormatter& getEventFormatter() {
		if (_customFormatter)
			return *_customFormatter;
		return _formatter;
	}

private:

	DefaultEventFormatter _formatter;
	StandaloneAdapter _standaloneAdapter;

	fakeit::EventFormatter * _customFormatter;
	fakeit::EventHandler* _testingFrameworkAdapter;
};
}

#endif //
