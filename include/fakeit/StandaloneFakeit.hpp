#pragma once

#include "fakeit/DefaultFakeit.hpp"

namespace fakeit {

    class StandaloneFakeit : public DefaultFakeit {

    public:
        virtual ~StandaloneFakeit() = default;

        StandaloneFakeit()
                : _formatter(),
                  _standaloneAdapter(*this) {
        }

        static StandaloneFakeit &getInstance() {
            static StandaloneFakeit instance;
            return instance;
        }
        
    protected:

        fakeit::EventHandler &accessTestingFrameworkAdapter() override {
            return _standaloneAdapter;
        }

        EventFormatter &accessEventFormatter() override {
            return _formatter;
        }

    private:

        DefaultEventFormatter _formatter;
        StandaloneAdapter _standaloneAdapter;

    };
}