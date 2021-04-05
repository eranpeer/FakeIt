#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include <cute.h>

namespace fakeit {

	struct CuteAdapter : public EventHandler {
		virtual ~CuteAdapter() = default;

		CuteAdapter(EventFormatter &formatter)
			: _formatter(formatter) {
		}

		virtual void handle(const UnexpectedMethodCallEvent &evt) override {
			std::string format = _formatter.format(evt);
			throw std::string(format);
        }

		virtual void handle(const SequenceVerificationEvent &evt) override {
			std::string format(_formatter.format(evt));
			throw cute::test_failure(format, evt.file(), evt.line());
        }

		virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
			std::string format = _formatter.format(evt);
			throw cute::test_failure(format, evt.file(), evt.line());
        }

	private:
		EventFormatter &_formatter;
	};

    class CuteFakeit : public DefaultFakeit {

    public:
        virtual ~CuteFakeit() = default;

        CuteFakeit(): _CuteAdapter(*this) {
        }

        static CuteFakeit &getInstance() {
            static CuteFakeit instance;
            return instance;
        }
        
    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _CuteAdapter;
        }

    private:

        CuteAdapter _CuteAdapter;
    };
}
