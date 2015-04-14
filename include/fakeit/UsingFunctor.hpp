/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <set>

#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/UsingProgress.hpp"

namespace fakeit {

    class UsingFunctor {

        friend class VerifyFunctor;

        FakeitContext &_fakeit;

    public:

        UsingFunctor(FakeitContext &fakeit) : _fakeit(fakeit) {
        }

        template<typename ... list>
        UsingProgress operator()(const ActualInvocationsSource &head, const list &... tail) {
            std::vector<ActualInvocationsSource *> allMocks{&InvocationUtils::remove_const(head),
                                                            &InvocationUtils::remove_const(tail)...};
            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(allMocks)};
            UsingProgress progress(_fakeit, aggregateInvocationsSource);
            return progress;
        }

    };
}
