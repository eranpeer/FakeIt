/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <functional>
#include <memory>

#include "fakeit/Sequence.hpp"
#include "fakeit/DomainObjects.hpp"

namespace fakeit {

    enum class VerificationType {
        Exact, AtLeast, NoMoreInvocations
    };

    enum class UnexpectedType {
        Unmocked, Unmatched
    };

    struct VerificationEvent {

        VerificationEvent(VerificationType verificationType) :
                _verificationType(verificationType), _line(0) {
        }

        virtual ~VerificationEvent() = default;

        VerificationType verificationType() const {
            return _verificationType;
        }

        void setFileInfo(std::string file, int line, std::string callingMethod) {
            _file = file;
            _callingMethod = callingMethod;
            _line = line;
        }

        std::string file() const {
            return _file;
        }

        int line() const {
            return _line;
        }

        const std::string &callingMethod() const {
            return _callingMethod;
        }

    private:
        VerificationType _verificationType;
        std::string _file;
        int _line;
        std::string _callingMethod;
    };

    struct NoMoreInvocationsVerificationEvent : public VerificationEvent {

        ~NoMoreInvocationsVerificationEvent() = default;

        NoMoreInvocationsVerificationEvent( //
                std::vector<Invocation *> &allIvocations, //
                std::vector<Invocation *> &unverifedIvocations) : //
                VerificationEvent(VerificationType::NoMoreInvocations), //
                _allIvocations(allIvocations), //
                _unverifedIvocations(unverifedIvocations) { //
        }

        const std::vector<Invocation *> &allIvocations() const {
            return _allIvocations;
        }

        const std::vector<Invocation *> &unverifedIvocations() const {
            return _unverifedIvocations;
        }

    private:
        const std::vector<Invocation *> _allIvocations;
        const std::vector<Invocation *> _unverifedIvocations;
    };

    struct SequenceVerificationEvent : public VerificationEvent {

        ~SequenceVerificationEvent() = default;

        SequenceVerificationEvent(VerificationType verificationType, //
                                  std::vector<Sequence *> &expectedPattern, //
                                  std::vector<Invocation *> &actualSequence, //
                                  int expectedCount, //
                                  int actualCount) : //
                VerificationEvent(verificationType), //
                _expectedPattern(expectedPattern), //
                _actualSequence(actualSequence), //
                _expectedCount(expectedCount), //
                _actualCount(actualCount) //
        { //
        }

        const std::vector<Sequence *> &expectedPattern() const {
            return _expectedPattern;
        }

        const std::vector<Invocation *> &actualSequence() const {
            return _actualSequence;
        }

        int expectedCount() const {
            return _expectedCount;
        }

        int actualCount() const {
            return _actualCount;
        }

    private:
        const std::vector<Sequence *> _expectedPattern;
        const std::vector<Invocation *> _actualSequence;
        const int _expectedCount;
        const int _actualCount;
    };

    struct UnexpectedMethodCallEvent {
        UnexpectedMethodCallEvent(UnexpectedType unexpectedType, const Invocation &invocation) :
                _unexpectedType(unexpectedType), _invocation(invocation) {
        }

        const Invocation &getInvocation() const {
            return _invocation;
        }

        UnexpectedType getUnexpectedType() const {
            return _unexpectedType;
        }

        const UnexpectedType _unexpectedType;
        const Invocation &_invocation;
    };

}
