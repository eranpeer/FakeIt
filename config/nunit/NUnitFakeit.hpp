#pragma once

#include "fakeit/DefaultFakeit.hpp"

namespace fakeit {

	struct NUnitAdapter : public EventHandler {
		virtual ~NUnitAdapter() = default;

		NUnitAdapter(EventFormatter &formatter)
			: _formatter(formatter) {
		}

		virtual void handle(const UnexpectedMethodCallEvent &evt) override {
			std::string format = _formatter.format(evt);
			NUnit::Framework::Assert::Fail(gcnew String(format.c_str()));
		}

		virtual void handle(const SequenceVerificationEvent &evt) override {
			std::string format(_formatter.format(evt));
			//The GTest example I copied used evt.file(), evt.line(), and ::testing::TestPartResult::kFatalFailure
			//I am not here for simplicity. 
			NUnit::Framework::Assert::Fail(gcnew String(format.c_str()));
	        }

		virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
			std::string format = _formatter.format(evt);
			//The GTest example I copied used evt.file(), evt.line(), and ::testing::TestPartResult::kFatalFailure
			//I am not here for simplicity. 
			NUnit::Framework::Assert::Fail(gcnew String(format.c_str()));
	        }

	private:
		EventFormatter &_formatter;
	};

    class NUnitFakeit : public DefaultFakeit {

    public:
        virtual ~NUnitFakeit() = default;

        NUnitFakeit(): _nUnitAdapter(*this) {
        }

        static NUnitFakeit &getInstance() {
            static NUnitFakeit instance;
            return instance;
        }
        
    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _nUnitAdapter;
        }

    private:

        NUnitAdapter _nUnitAdapter;
    };
}
