//
// Created by eran on 02/04/2015.
//

#ifndef _TESTS_UNVERIFIEDFUNCTOR_HPP_
#define _TESTS_UNVERIFIEDFUNCTOR_HPP_

#include <vector>
#include <set>
#include <fakeit/SortInvocations.hpp>

namespace fakeit {
    class VerifyUnverifiedFunctor {

        FakeitContext &_fakeit;

    public:

        VerifyUnverifiedFunctor(FakeitContext &fakeit) : _fakeit(fakeit) {
        }

        template<typename ... list>
        SequenceVerificationProgress operator()(const Sequence &sequence, const list &... tail) {
            std::vector<Sequence *> allSequences;
            collectSequences(allSequences, sequence, tail...);

            std::set<ActualInvocationsSource *> invlovedSources;
            collectInvolvedMocks(allSequences, invlovedSources);

            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(invlovedSources)};
            InvocationsSourceProxy unverifiedInvocationsSource{new UnverifiedInvocationsSource(aggregateInvocationsSource)};

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
            std::set<ActualInvocationsSource *> allMocks;
            allMocks.insert(const_cast<ActualInvocationsSource *> (&head));
            collectInvocationSources(allMocks, tail...);
            InvocationsSourceProxy aggregateInvocationsSource{new AggregateInvocationsSource(allMocks)};
            UnverifiedInvocationsSource unverifiedInvocationsSource{aggregateInvocationsSource};
            return unverifiedInvocationsSource;
        }
    };
}
#endif //_TESTS_UNVERIFIEDFUNCTOR_HPP_
