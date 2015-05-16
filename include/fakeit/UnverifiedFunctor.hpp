//
// Created by eran on 02/04/2015.
//
#pragma once

#include <vector>
#include <set>
#include "fakeit/SortInvocations.hpp"

namespace fakeit {
    class VerifyUnverifiedFunctor {

        FakeitContext &_fakeit;

    public:

        VerifyUnverifiedFunctor(FakeitContext &fakeit) : _fakeit(fakeit) {
        }

        template<typename ... list>
        SequenceVerificationProgress operator()(const Sequence &sequence, const list &... tail) {
            std::vector<Sequence *> allSequences{&InvocationUtils::remove_const(sequence),
                                                 &InvocationUtils::remove_const(tail)...};

            std::vector<ActualInvocationsSource *> involvedSources;
            InvocationUtils::collectInvolvedMocks(allSequences, involvedSources);

            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(involvedSources)};
            InvocationsSourceProxy unverifiedInvocationsSource{
                    new UnverifiedInvocationsSource(aggregateInvocationsSource)};

            UsingProgress usingProgress(_fakeit, unverifiedInvocationsSource);
            return usingProgress.Verify(sequence, tail...);
        }

    };

    class UnverifiedFunctor {
    public:
        UnverifiedFunctor(FakeitContext &fakeit) : Verify(fakeit) {
        }

        VerifyUnverifiedFunctor Verify;

        template<typename ... list>
        UnverifiedInvocationsSource operator()(const ActualInvocationsSource &head, const list &... tail) {
            std::vector<ActualInvocationsSource *> allMocks{&InvocationUtils::remove_const(head),
                                                            &InvocationUtils::remove_const(tail)...};
            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(allMocks)};
            UnverifiedInvocationsSource unverifiedInvocationsSource{aggregateInvocationsSource};
            return unverifiedInvocationsSource;
        }

//        template<typename ... list>
//        void operator()(const Sequence &sequence, const list &... tail) {
//            std::vector<Sequence *> allSequences;
//            collectSequences(allSequences, sequence, tail...);
//
//            std::vector<ActualInvocationsSource *> involvedSources;
//            collectInvolvedMocks(allSequences, involvedSources);
//
//            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(involvedSources)};
//            InvocationsSourceProxy unverifiedInvocationsSource{new UnverifiedInvocationsSource(aggregateInvocationsSource)};
//        }

    };
}
