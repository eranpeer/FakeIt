#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include <boost/test/unit_test.hpp>

#define BOOST_TEST_TOOL_IMPL2( func, P, check_descr, TL, CT, F, L )            \
    ::boost::test_tools::tt_detail::func(                               \
        P,                                                              \
        ::boost::unit_test::lazy_ostream::instance() << check_descr,    \
        F,                                         \
        static_cast<std::size_t>(L),                             \
        ::boost::test_tools::tt_detail::TL,                             \
        ::boost::test_tools::tt_detail::CT      , 0)                        \

namespace fakeit {

	struct BoostTestAdapter : public EventHandler {
		virtual ~BoostTestAdapter() = default;

        BoostTestAdapter(EventFormatter &formatter)
			: _formatter(formatter) {
		}

		virtual void handle(const UnexpectedMethodCallEvent &evt) override {
            std::string format = _formatter.format(evt);
            throw format;
        }

		virtual void handle(const SequenceVerificationEvent &evt) override {
            std::string format = _formatter.format(evt);
            boost_fail(evt.file(), evt.line(), format);
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            std::string format = _formatter.format(evt);
            boost_fail(evt.file(), evt.line(), format);
        }


	private:
		EventFormatter &_formatter;

        void boost_fail(std::string file, int line, std::string format){
            //::boost::unit_test::unit_test_log.set_checkpoint(
            //    file,
            //    static_cast<std::size_t>(line));

            ::boost::test_tools::tt_detail::check_impl(
                false,
                ::boost::unit_test::lazy_ostream::instance() << format,
                file,
                static_cast<std::size_t>(line),
                ::boost::test_tools::tt_detail::REQUIRE,
                ::boost::test_tools::tt_detail::CHECK_MSG, 0);
        }
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