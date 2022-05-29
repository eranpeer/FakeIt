#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"
#include "mockutils/to_string.hpp"
#if defined __has_include
#   if __has_include("catch2/catch.hpp")
#      include <catch2/catch.hpp>
#   elif __has_include("catch2/catch_all.hpp")
#      include <catch2/catch_assertion_result.hpp>
#      include <catch2/catch_test_macros.hpp>
#   elif __has_include("catch_amalgamated.hpp")
#      include <catch_amalgamated.hpp>
#   else
#      include <catch.hpp>
#   endif
#else
#   include <catch2/catch.hpp>
#endif

namespace fakeit {

    class CatchAdapter : public EventHandler {
        EventFormatter &_formatter;

        std::string formatLineNumber(std::string file, int num) {
#ifndef __GNUG__
            return file + std::string("(") + fakeit::to_string(num) + std::string(")");
#else
            return file + std::string(":") + fakeit::to_string(num);
#endif
        }

    public:

        virtual ~CatchAdapter() = default;

        CatchAdapter(EventFormatter &formatter)
                : _formatter(formatter) {}

        void fail(
                std::string vetificationType,
                Catch::SourceLineInfo sourceLineInfo,
                std::string failingExpression,
                std::string fomattedMessage,
                Catch::ResultWas::OfType resultWas = Catch::ResultWas::OfType::ExpressionFailed ){
            Catch::AssertionHandler catchAssertionHandler( vetificationType, sourceLineInfo, failingExpression, Catch::ResultDisposition::Normal );
#if defined(CATCH_INTERNAL_START_WARNINGS_SUPPRESSION) && defined(CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION)
            INTERNAL_CATCH_TRY { \
                CATCH_INTERNAL_START_WARNINGS_SUPPRESSION \
                CATCH_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS \
                catchAssertionHandler.handleMessage(resultWas, fomattedMessage); \
                CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION \
            } INTERNAL_CATCH_CATCH(catchAssertionHandler) { \
                INTERNAL_CATCH_REACT(catchAssertionHandler) \
            }
#else
            INTERNAL_CATCH_TRY { \
                CATCH_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS \
                catchAssertionHandler.handleMessage(resultWas, fomattedMessage); \
                CATCH_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS \
            } INTERNAL_CATCH_CATCH(catchAssertionHandler) { \
                INTERNAL_CATCH_REACT(catchAssertionHandler) \
            }
#endif
        }

        virtual void handle(const UnexpectedMethodCallEvent &evt) override {
            std::string format = _formatter.format(evt);
            fail("UnexpectedMethodCall",::Catch::SourceLineInfo("Unknown file",0),"",format, Catch::ResultWas::OfType::ExplicitFailure);
        }

        virtual void handle(const SequenceVerificationEvent &evt) override {
            std::string format(formatLineNumber(evt.file(), evt.line()) + ": " + _formatter.format(evt));
            std::string expectedPattern {DefaultEventFormatter::formatExpectedPattern(evt.expectedPattern())};
            fail("Verify",::Catch::SourceLineInfo(evt.file(),evt.line()),expectedPattern,format);
        }


        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            std::string format(formatLineNumber(evt.file(), evt.line()) + ": " + _formatter.format(evt));
            fail("VerifyNoMoreInvocations",::Catch::SourceLineInfo(evt.file(),evt.line()),"",format);
        }

    };


    class CatchFakeit : public DefaultFakeit {


    public:

        virtual ~CatchFakeit() = default;

        CatchFakeit() : _formatter(), _catchAdapter(_formatter) {}

        static CatchFakeit &getInstance() {
            static CatchFakeit instance;
            return instance;
        }

    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _catchAdapter;
        }

        EventFormatter &accessEventFormatter() override {
            return _formatter;
        }

    private:

        DefaultEventFormatter _formatter;
        CatchAdapter _catchAdapter;
    };

}
