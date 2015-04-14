/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#include "fakeit/EventHandler.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/DefaultEventLogger.hpp"
#include "fakeit/DefaultEventFormatter.hpp"

namespace fakeit {

    class AbstractFakeit : public FakeitContext {
    public:
        virtual ~AbstractFakeit() = default;

    protected:

        virtual fakeit::EventHandler &accessTestingFrameworkAdapter() = 0;

        virtual EventFormatter &accessEventFormatter() = 0;
    };

    class DefaultFakeit : public AbstractFakeit {
        DefaultEventFormatter _formatter;
        fakeit::EventFormatter *_customFormatter;
        fakeit::EventHandler *_testingFrameworkAdapter;

    public:

        DefaultFakeit() : _formatter(),
                          _customFormatter(nullptr),
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

        EventFormatter &accessEventFormatter() override {
            return _formatter;
        }

    };
}
