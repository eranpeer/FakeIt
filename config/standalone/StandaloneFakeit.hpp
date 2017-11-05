#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "mockutils/to_string.hpp"

namespace fakeit {

    struct VerificationException : public std::exception {
        virtual ~VerificationException() NO_THROWS{};
        
        VerificationException(std::string format) : //
            _format(format) { //
        }

        friend std::ostream &operator<<(std::ostream &os, const VerificationException &val) {
            os << val.what();
            return os;
        }

        void setFileInfo(std::string aFile, int aLine, std::string aCallingMethod) {
            _file = aFile;
            _callingMethod = aCallingMethod;
            _line = aLine;
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

        const char* what() const NO_THROWS override{
            return _format.c_str();
        }
    private:
        std::string _file;
        int _line;
        std::string _callingMethod;
        std::string _format;
    };

    struct NoMoreInvocationsVerificationException : public VerificationException {
        NoMoreInvocationsVerificationException(std::string format) : //
            VerificationException(format) { //
        }
    };

    struct SequenceVerificationException : public VerificationException {
        SequenceVerificationException(std::string format) : //
            VerificationException(format) { //
        }
    };

    struct StandaloneAdapter : public EventHandler {

        std::string formatLineNumner(std::string file, int num){
#ifndef __GNUG__
            return file + std::string("(") + fakeit::to_string(num) + std::string(")");
#else
            return file + std::string(":") + fakeit::to_string(num);
#endif
        }

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
            std::string format(formatLineNumner(evt.file(), evt.line()) + ": " + _formatter.format(evt));
            SequenceVerificationException e(format);
            e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
            throw e;
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            std::string format(formatLineNumner(evt.file(), evt.line()) + ": " + _formatter.format(evt));
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
