#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"
#include "catch.hpp"

namespace fakeit {

    struct VerificationException : public FakeitException {
        virtual ~VerificationException() = default;

        void setFileInfo(const char *file, int line, const char *callingMethod) {
            _file = file;
            _callingMethod = callingMethod;
            _line = line;
        }

        const char *file() const {
            return _file;
        }

        int line() const {
            return _line;
        }

        const char *callingMethod() const {
            return _callingMethod;
        }

    private:
        const char *_file;
        int _line;
        const char *_callingMethod;
    };

    struct NoMoreInvocationsVerificationException : public VerificationException {

        NoMoreInvocationsVerificationException(std::string format) : //
                _format(format) { //
        }

        virtual std::string what() const override {
            return _format;
        }

    private:
        std::string _format;
    };

    struct SequenceVerificationException : public VerificationException {
        SequenceVerificationException(const std::string &format) : //
                _format(format) //
        {
        }

        virtual std::string what() const override {
            return _format;
        }

    private:
        std::string _format;
    };

    class CatchAdapter : public EventHandler {
        EventFormatter &_formatter;

        std::string formatLineNumber(std::string file, int num) {
#ifndef __GNUG__
            return file + std::string("(") + std::to_string(num) + std::string(")");
#else
            return file + std::string(":") + std::to_string(num);
#endif
        }

    public:

        virtual ~CatchAdapter() = default;

        CatchAdapter(EventFormatter &formatter)
                : _formatter(formatter) {}

        virtual void handle(const UnexpectedMethodCallEvent &evt) override {
            std::string format = _formatter.format(evt);
            Catch::ResultBuilder __catchResult("FAIL", ::Catch::SourceLineInfo(),
                                               "", Catch::ResultDisposition::Normal);
            __catchResult << format + ::Catch::StreamEndStop();
            __catchResult.captureResult(Catch::ResultWas::ExplicitFailure);
            INTERNAL_CATCH_REACT(__catchResult)
            throw Catch::TestFailureException();
        }

        virtual void handle(const SequenceVerificationEvent &evt) override {
            std::string format(formatLineNumber(evt.file(), evt.line()) + ": " + _formatter.format(evt));
            Catch::ResultBuilder __catchResult("FAIL", ::Catch::SourceLineInfo(evt.file(),
                                                                               static_cast<std::size_t>( evt.line())),
                                               "", Catch::ResultDisposition::Normal);
            __catchResult << format + ::Catch::StreamEndStop();
            __catchResult.captureResult(Catch::ResultWas::ExplicitFailure);
            INTERNAL_CATCH_REACT(__catchResult)
            throw Catch::TestFailureException();
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            std::string format(formatLineNumber(evt.file(), evt.line()) + ": " + _formatter.format(evt));
            Catch::ResultBuilder __catchResult("FAIL", ::Catch::SourceLineInfo(evt.file(),
                                                                               static_cast<std::size_t>( evt.line())),
                                               "", Catch::ResultDisposition::Normal);
            __catchResult << format + ::Catch::StreamEndStop();
            __catchResult.captureResult(Catch::ResultWas::ExplicitFailure);
            INTERNAL_CATCH_REACT(__catchResult)
            throw Catch::TestFailureException();
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