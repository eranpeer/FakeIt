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

        VerificationEvent(VerificationType aVerificationType) :
                _verificationType(aVerificationType), _line(0) {
        }

        virtual ~VerificationEvent() = default;

        VerificationType verificationType() const {
            return _verificationType;
        }

        void setFileInfo(const char * aFile, int aLine, const char * aCallingMethod) {
            _file = aFile;
            _callingMethod = aCallingMethod;
            _line = aLine;
        }

        const char * file() const {
            return _file;
        }

        int line() const {
            return _line;
        }

        const char * callingMethod() const {
            return _callingMethod;
        }

    private:
        VerificationType _verificationType;
		const char * _file;
        int _line;
        const char * _callingMethod;
    };

    struct NoMoreInvocationsVerificationEvent : public VerificationEvent {

        ~NoMoreInvocationsVerificationEvent() = default;

        NoMoreInvocationsVerificationEvent( //
                std::vector<Invocation *> &allTheIvocations, //
                std::vector<Invocation *> &anUnverifedIvocations) : //
                VerificationEvent(VerificationType::NoMoreInvocations), //
                _allIvocations(allTheIvocations), //
                _unverifedIvocations(anUnverifedIvocations) { //
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

        SequenceVerificationEvent(VerificationType aVerificationType, //
                                  std::vector<Sequence *> &anExpectedPattern, //
                                  std::vector<Invocation *> &anActualSequence, //
                                  int anExpectedCount, //
                                  int anActualCount) : //
                VerificationEvent(aVerificationType), //
                _expectedPattern(anExpectedPattern), //
                _actualSequence(anActualSequence), //
                _expectedCount(anExpectedCount), //
                _actualCount(anActualCount) //
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
