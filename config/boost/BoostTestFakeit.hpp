#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include <boost/test/unit_test.hpp>
namespace fakeit {

	struct BoostTestAdapter : public EventHandler {
		virtual ~BoostTestAdapter() = default;

        BoostTestAdapter(EventFormatter &formatter)
			: _formatter(formatter) {
		}

		virtual void handle(const UnexpectedMethodCallEvent &evt) override {
			std::string format = _formatter.format(evt);
            BOOST_FAIL(format);
        }

		virtual void handle(const SequenceVerificationEvent &evt) override {
			std::string format(_formatter.format(evt));
            BOOST_FAIL(format);
        }

		virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
			std::string format = "\n" + _formatter.format(evt);
            BOOST_FAIL(format);
        }

	private:
		EventFormatter &_formatter;
	};

    class BoostTestFakeit : public DefaultFakeit {

    public:
        virtual ~BoostTestFakeit() = default;

        BoostTestFakeit(): _boostTestAdapter(*this) {
        }

        static BoostTestFakeit &getInstance() {
            static BoostTestFakeit instance;
            return instance;
        }
        
    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _boostTestAdapter;
        }

    private:

        BoostTestAdapter _boostTestAdapter;
    };
}