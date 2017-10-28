#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "mockutils/to_string.hpp"

namespace fakeit {

    struct MettleAdapter : public EventHandler {

        std::string formatLineNumner(std::string file, int num){
#ifndef __GNUG__
            return file + std::string("(") + fakeit::to_string(num) + std::string(")");
#else
            return file + std::string(":") + fakeit::to_string(num);
#endif
        }

        virtual ~MettleAdapter() = default;

        MettleAdapter(EventFormatter &formatter)
            : _formatter(formatter) {
        }

        virtual void handle(const UnexpectedMethodCallEvent &evt) override {
            throw mettle::expectation_error(_formatter.format(evt));
        }

        virtual void handle(const SequenceVerificationEvent &evt) override {
            throw mettle::expectation_error(
                    formatLineNumner(evt.file(), evt.line()) + ": " + 
                    _formatter.format(evt));
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
            throw mettle::expectation_error(
                    formatLineNumner(evt.file(), evt.line()) + ": " + 
                    _formatter.format(evt));
        }

    private:
        EventFormatter &_formatter;
    };

    class MettleFakeit : public DefaultFakeit {

    public:
        virtual ~MettleFakeit() = default;

        MettleFakeit() : _mettleAdapter(*this) {
        }

        static MettleFakeit &getInstance() {
            static MettleFakeit instance;
            return instance;
        }

    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _mettleAdapter;
        }

    private:

        MettleAdapter _mettleAdapter;
    };
}
