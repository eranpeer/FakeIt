#pragma once

#include "mockutils/smart_ptr.hpp"
#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/MatchAnalysis.hpp"

namespace fakeit {

    struct SequenceVerificationExpectation {

        friend class SequenceVerificationProgress;

        ~SequenceVerificationExpectation() THROWS {
            if (std::uncaught_exception()) {
                return;
            }
            VerifyExpectation(_fakeit);
        }

        void setExpectedPattern(std::vector<Sequence *> expectedPattern) {
            _expectedPattern = expectedPattern;
        }

        void setExpectedCount(const int count) {
            _expectedCount = count;
        }

        void expectAnything() {
            _expectAnything = true;
        }

        void setFileInfo(std::string file, int line, std::string callingMethod) {
            _file = file;
            _line = line;
            _testMethod = callingMethod;
        }

    private:

        VerificationEventHandler &_fakeit;
        InvocationsSourceProxy _involvedInvocationSources;
        std::vector<Sequence *> _expectedPattern;
        int _expectedCount;
        bool _expectAnything;

        std::string _file;
        int _line;
        std::string _testMethod;
        bool _isVerified;

        SequenceVerificationExpectation(
                VerificationEventHandler &fakeit,
                InvocationsSourceProxy mocks,
                std::vector<Sequence *> &expectedPattern) : //
                _fakeit(fakeit),
                _involvedInvocationSources(mocks),
                _expectedPattern(expectedPattern), //
                _expectedCount(-1), // AT_LEAST_ONCE
                _expectAnything(false),
                _line(0),
                _isVerified(false) {
        }


        void VerifyExpectation(VerificationEventHandler &verificationErrorHandler) {
            if (_isVerified)
                return;
            _isVerified = true;

            MatchAnalysis ma;
            ma.run(_involvedInvocationSources, _expectedPattern);

            if (isNotAnythingVerification()) {
                if (isAtLeastVerification() && atLeastLimitNotReached(ma.count)) {
                    return handleAtLeastVerificationEvent(verificationErrorHandler, ma.actualSequence, ma.count);
                }

                if (isExactVerification() && exactLimitNotMatched(ma.count)) {
                    return handleExactVerificationEvent(verificationErrorHandler, ma.actualSequence, ma.count);
                }
            }

            markAsVerified(ma.matchedInvocations);
        }

        std::vector<Sequence *> &collectSequences(std::vector<Sequence *> &vec) {
            return vec;
        }

        template<typename ... list>
        std::vector<Sequence *> &collectSequences(std::vector<Sequence *> &vec, const Sequence &sequence,
                                                  const list &... tail) {
            vec.push_back(&const_cast<Sequence &>(sequence));
            return collectSequences(vec, tail...);
        }


        static void markAsVerified(std::vector<Invocation *> &matchedInvocations) {
            for (auto i : matchedInvocations) {
                i->markAsVerified();
            }
        }

        bool isNotAnythingVerification() {
            return !_expectAnything;
        }

        bool isAtLeastVerification() {
            // negative number represents an "AtLeast" search;
            return _expectedCount < 0;
        }

        bool isExactVerification() {
            return !isAtLeastVerification();
        }

        bool atLeastLimitNotReached(int count) {
            return count < -_expectedCount;
        }

        bool exactLimitNotMatched(int count) {
            return count != _expectedCount;
        }

        void handleExactVerificationEvent(VerificationEventHandler &verificationErrorHandler,
                                          std::vector<Invocation *> actualSequence, int count) {
            SequenceVerificationEvent evt(VerificationType::Exact, _expectedPattern, actualSequence, _expectedCount,
                                          count);
            evt.setFileInfo(_file, _line, _testMethod);
            return verificationErrorHandler.handle(evt);
        }

        void handleAtLeastVerificationEvent(VerificationEventHandler &verificationErrorHandler,
                                            std::vector<Invocation *> actualSequence, int count) {
            SequenceVerificationEvent evt(VerificationType::AtLeast, _expectedPattern, actualSequence, -_expectedCount,
                                          count);
            evt.setFileInfo(_file, _line, _testMethod);
            return verificationErrorHandler.handle(evt);
        }

    };

}
