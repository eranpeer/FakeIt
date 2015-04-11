#pragma once

#include "fakeit/DefaultFakeit.hpp"

namespace fakeit {

    struct VerificationException : public FakeitException {
        virtual ~VerificationException() = default;

        void setFileInfo(std::string file, int line, std::string callingMethod) {
            _file = file;
            _callingMethod = callingMethod;
            _line = line;
        }

        const std::string& file() const {
            return _file;
        }
        int line() const {
            return _line;
        }
        const std::string& callingMethod() const {
            return _callingMethod;
        }

    private:
        std::string _file;
        int _line;
        std::string _callingMethod;
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
        SequenceVerificationException(const std::string& format) : //
            _format(format) //
        {
        }

        virtual std::string what() const override {
            return _format;
        }

    private:
        std::string _format;
    };

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
            std::string format(evt.file() + ":" + std::to_string(evt.line()) + ": " + _formatter.format(evt));
            SequenceVerificationException e(format);
            e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
            throw e;
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            std::string format = evt.file() + ":" + std::to_string(evt.line()) + ": " + _formatter.format(evt);
            NoMoreInvocationsVerificationException e(format);
            e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
            throw e;
        }

    private:
        EventFormatter &_formatter;
    };

    class StandaloneFakeit : public DefaultFakeit {

    public:
        virtual ~StandaloneFakeit() = default;

        StandaloneFakeit() : _standaloneAdapter(*this) {
        }

        static StandaloneFakeit &getInstance() {
            static StandaloneFakeit instance;
            return instance;
        }

    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _standaloneAdapter;
        }

    private:

        StandaloneAdapter _standaloneAdapter;
    };
}
