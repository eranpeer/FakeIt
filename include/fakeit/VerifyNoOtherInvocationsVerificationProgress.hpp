/*
 * VerifyNoOtherInvocationsVerificationProgress.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jul 21, 2014
 */
#pragma once

#include "fakeit/FakeitContext.hpp"
#include "fakeit/ThrowFalseEventHandler.hpp"

namespace fakeit {

    class VerifyNoOtherInvocationsVerificationProgress {

        friend class VerifyNoOtherInvocationsFunctor;

        struct VerifyNoOtherInvocationsExpectation {

            friend class VerifyNoOtherInvocationsVerificationProgress;

            ~VerifyNoOtherInvocationsExpectation() THROWS {
                if (std::uncaught_exception()) {
                    return;
                }

                VerifyExpectation(_fakeit);
            }

            void setFileInfo(std::string file, int line, std::string callingMethod) {
                _file = file;
                _line = line;
                _callingMethod = callingMethod;
            }

        private:

            VerificationEventHandler &_fakeit;
            std::vector<ActualInvocationsSource *> _mocks;

            std::string _file;
            int _line;
            std::string _callingMethod;
            bool _isVerified;

            VerifyNoOtherInvocationsExpectation(VerificationEventHandler &fakeit,
                                                std::vector<ActualInvocationsSource *> mocks) :
                    _fakeit(fakeit),
                    _mocks(mocks),
                    _line(0),
                    _isVerified(false) {
            }

            VerifyNoOtherInvocationsExpectation(VerifyNoOtherInvocationsExpectation &other) = default;

            void VerifyExpectation(VerificationEventHandler &verificationErrorHandler) {
                if (_isVerified)
                    return;
                _isVerified = true;

                std::unordered_set<Invocation *> actualInvocations;
                InvocationUtils::collectActualInvocations(actualInvocations, _mocks);

                std::unordered_set<Invocation *> nonVerifiedInvocations;
                InvocationUtils::selectNonVerifiedInvocations(actualInvocations, nonVerifiedInvocations);

                if (nonVerifiedInvocations.size() > 0) {
                    std::vector<Invocation *> sortedNonVerifiedInvocations;
                    InvocationUtils::sortByInvocationOrder(nonVerifiedInvocations, sortedNonVerifiedInvocations);

                    std::vector<Invocation *> sortedActualInvocations;
                    InvocationUtils::sortByInvocationOrder(actualInvocations, sortedActualInvocations);

                    NoMoreInvocationsVerificationEvent evt(sortedActualInvocations, sortedNonVerifiedInvocations);
                    evt.setFileInfo(_file, _line, _callingMethod);
                    return verificationErrorHandler.handle(evt);
                }
            }

        };

        fakeit::smart_ptr<VerifyNoOtherInvocationsExpectation> _ptr;

        VerifyNoOtherInvocationsVerificationProgress(VerifyNoOtherInvocationsExpectation *ptr) :
                _ptr(ptr) {
        }

        VerifyNoOtherInvocationsVerificationProgress(FakeitContext &fakeit,
                                                     std::vector<ActualInvocationsSource *> &invocationSources)
                : VerifyNoOtherInvocationsVerificationProgress(
                new VerifyNoOtherInvocationsExpectation(fakeit, invocationSources)
        ) {
        }

        bool toBool() {
            try {
                ThrowFalseEventHandler ev;
                _ptr->VerifyExpectation(ev);
                return true;
            }
            catch (bool e) {
                return e;
            }
        }

    public:


        ~VerifyNoOtherInvocationsVerificationProgress() THROWS {
        };

        VerifyNoOtherInvocationsVerificationProgress setFileInfo(std::string file, int line,
                                                                 std::string callingMethod) {
            _ptr->setFileInfo(file, line, callingMethod);
            return *this;
        }

        operator bool() {
            return toBool();
        }

        bool operator!() const { return !const_cast<VerifyNoOtherInvocationsVerificationProgress *>(this)->toBool(); }

    };

}
