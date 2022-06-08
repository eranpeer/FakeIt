#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"
#include "mockutils/to_string.hpp"
#include <doctest.h>

namespace fakeit
{
    class DoctestAdapter : public EventHandler
    {
        EventFormatter &_formatter;

    public:
        virtual ~DoctestAdapter() = default;

        DoctestAdapter(EventFormatter &formatter)
                : _formatter(formatter) {}

        void fail(const char* fileName,
                  int lineNumber,
                  std::string fomattedMessage,
                  bool fatalFailure)
        {
            if (fatalFailure)
            {
                DOCTEST_ADD_FAIL_AT(fileName, lineNumber, fomattedMessage);
            }
            else
            {
                DOCTEST_ADD_FAIL_CHECK_AT(fileName, lineNumber, fomattedMessage);
            }
        }

        void handle(const UnexpectedMethodCallEvent &evt) override
        {
            fail("Unknown file", 0, _formatter.format(evt), true);
        }

        void handle(const SequenceVerificationEvent &evt) override
        {
            fail(evt.file(), evt.line(), _formatter.format(evt), false);
        }

        void handle(const NoMoreInvocationsVerificationEvent &evt) override
        {
            fail(evt.file(), evt.line(), _formatter.format(evt), false);
        }
    };

    class DoctestFakeit : public DefaultFakeit
    {
    public:
        virtual ~DoctestFakeit() = default;

        DoctestFakeit() : _doctestAdapter(*this) {}

        static DoctestFakeit &getInstance()
        {
            static DoctestFakeit instance;
            return instance;
        }

    protected:
        fakeit::EventHandler &accessTestingFrameworkAdapter() override
        {
            return _doctestAdapter;
        }

    private:
        DoctestAdapter _doctestAdapter;
    };
}
