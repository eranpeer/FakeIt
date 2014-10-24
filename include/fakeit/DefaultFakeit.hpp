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

    struct StandaloneAdapter : public EventHandler {
        virtual ~StandaloneAdapter() = default;

        StandaloneAdapter(EventFormatter &formatter)
                : _formatter(formatter) {
        }

        virtual void handle(const UnexpectedMethodCallEvent &evt) override {
            std::string format = _formatter.format(evt);
            UnexpectedMethodCallException ex(format);
            throw ex;
        }

        virtual void handle(const SequenceVerificationEvent &evt) override {
            std::string format(_formatter.format(evt));
            SequenceVerificationException e(format);
            e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
            throw e;
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            std::string format = _formatter.format(evt);
            NoMoreInvocationsVerificationException e(format);
            e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
            throw e;
        }

    private:
        EventFormatter &_formatter;
    };

    class AbstractFakeit : public FakeitContext {
    public:
        virtual ~AbstractFakeit() = default;

    protected:

        virtual fakeit::EventHandler &accessTestingFrameworkAdapter() = 0;

        virtual EventFormatter &accessEventFormatter() = 0;
    };

    class DefaultFakeit : public AbstractFakeit {
        fakeit::EventFormatter *_customFormatter;
        fakeit::EventHandler *_testingFrameworkAdapter;
    public:

        DefaultFakeit() : _customFormatter(nullptr),
                          _testingFrameworkAdapter(nullptr) {
        }

        virtual ~DefaultFakeit() = default;

        void setCustomEventFormatter(fakeit::EventFormatter &customEventFormatter) {
            _customFormatter = &customEventFormatter;
        }

        void resetCustomEventFormatter() {
            _customFormatter = nullptr;
        }

        void setTestingFrameworkAdapter(fakeit::EventHandler &testingFrameforkAdapter) {
            _testingFrameworkAdapter = &testingFrameforkAdapter;
        }

        void resetTestingFrameworkAdapter() {
            _testingFrameworkAdapter = nullptr;
        }

    protected:

        fakeit::EventHandler &getTestingFrameworkAdapter() override {
            if (_testingFrameworkAdapter)
                return *_testingFrameworkAdapter;
            return accessTestingFrameworkAdapter();
        }

        EventFormatter &getEventFormatter() override {
            if (_customFormatter)
                return *_customFormatter;
            return accessEventFormatter();
        }
    };

}

#endif //
