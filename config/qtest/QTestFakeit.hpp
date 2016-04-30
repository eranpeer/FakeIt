#pragma once

#include <QTest>
#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"

namespace fakeit {

class QTestAdapter: public EventHandler {
    EventFormatter& _formatter;
public:

    virtual ~QTestAdapter() = default;
    QTestAdapter(EventFormatter& formatter):_formatter(formatter){}

    virtual void handle(const UnexpectedMethodCallEvent& e) override
    {
        auto str = _formatter.format(e);
        QFAIL(str.c_str());
    }

    virtual void handle(const SequenceVerificationEvent& e) override
    {
        auto str = _formatter.format(e);
        QFAIL(str.c_str());
    }

    virtual void handle(const NoMoreInvocationsVerificationEvent& e) override
    {
        auto str = _formatter.format(e);
        QFAIL(str.c_str());
    }

};

class QTestFakeit: public DefaultFakeit {

public:
    virtual ~QTestFakeit() = default;

    QTestFakeit()
            : _formatter(), _qtestAdapter(*this) {
    }

    static QTestFakeit &getInstance() {
        static QTestFakeit instance;
        return instance;
    }

protected:

    fakeit::EventHandler &accessTestingFrameworkAdapter() override {
        return _qtestAdapter;
    }

    EventFormatter &accessEventFormatter() override {
        return _formatter;
    }

private:

    DefaultEventFormatter _formatter;
    QTestAdapter _qtestAdapter;

};

}
