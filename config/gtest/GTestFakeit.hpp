#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "gtest/gtest.h"

namespace fakeit {

	struct GTestAdapter : public EventHandler {
		virtual ~GTestAdapter() = default;

        GTestAdapter(EventFormatter &formatter)
			: _formatter(formatter) {
		}

		virtual void handle(const UnexpectedMethodCallEvent &evt) override {
			std::string format = _formatter.format(evt);
            GTEST_FATAL_FAILURE_(format.c_str());
        }

		virtual void handle(const SequenceVerificationEvent &evt) override {
			std::string format(_formatter.format(evt));
			GTEST_MESSAGE_AT_(evt.file().c_str(), evt.line(), format.c_str(), ::testing::TestPartResult::kFatalFailure);
        }

		virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
			std::string format = _formatter.format(evt);
			GTEST_MESSAGE_AT_(evt.file().c_str(), evt.line(), format.c_str(), ::testing::TestPartResult::kFatalFailure);
        }

	private:
		EventFormatter &_formatter;
	};

    class GTestFakeit : public DefaultFakeit {

    public:
        virtual ~GTestFakeit() = default;

        GTestFakeit(): _gTestAdapter(*this) {
        }

        static GTestFakeit &getInstance() {
            static GTestFakeit instance;
            return instance;
        }
        
    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _gTestAdapter;
        }

    private:

        GTestAdapter _gTestAdapter;
    };
}
