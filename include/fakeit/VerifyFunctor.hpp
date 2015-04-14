/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <set>
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/UsingProgress.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/SequenceVerificationProgress.hpp"

namespace fakeit {

    class VerifyFunctor {

        FakeitContext &_fakeit;


    public:

        VerifyFunctor(FakeitContext &fakeit) : _fakeit(fakeit) {
        }

        template<typename ... list>
        SequenceVerificationProgress operator()(const Sequence &sequence, const list &... tail) {
            std::vector<Sequence *> allSequences{&InvocationUtils::remove_const(sequence),
                                                 &InvocationUtils::remove_const(tail)...};

            std::vector<ActualInvocationsSource *> involvedSources;
            InvocationUtils::collectInvolvedMocks(allSequences, involvedSources);
            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(involvedSources)};

            UsingProgress usingProgress(_fakeit, aggregateInvocationsSource);
            return usingProgress.Verify(sequence, tail...);
        }

    };

}
