//
// Created by eran on 01/04/2015.
//
#pragma once

#include "fakeit/FakeitContext.hpp"
#include "fakeit/SequenceVerificationProgress.hpp"

namespace fakeit {

    class UsingProgress {
        fakeit::FakeitContext &_fakeit;
        InvocationsSourceProxy _sources;

        void collectSequences(std::vector<fakeit::Sequence *> &) {
        }

        template<typename ... list>
        void collectSequences(std::vector<fakeit::Sequence *> &vec, const fakeit::Sequence &sequence,
                              const list &... tail) {
            vec.push_back(&const_cast<fakeit::Sequence &>(sequence));
            collectSequences(vec, tail...);
        }

    public:

        UsingProgress(fakeit::FakeitContext &fakeit, InvocationsSourceProxy source) :
                _fakeit(fakeit),
                _sources(source) {
        }

        template<typename ... list>
        SequenceVerificationProgress Verify(const fakeit::Sequence &sequence, const list &... tail) {
            std::vector<fakeit::Sequence *> allSequences;
            collectSequences(allSequences, sequence, tail...);
            SequenceVerificationProgress progress(_fakeit, _sources, allSequences);
            return progress;
        }

    };
}
