/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeitExceptions_h__
#define FakeitExceptions_h__

#include <functional>
#include "fakeit/Sequence.hpp"
#include "fakeit/DomainObjects.hpp"
#include "mockutils/Formatter.hpp"

namespace fakeit {

class FakeitException {
};

struct UnexpectedMethodCallException: public FakeitException {
	UnexpectedMethodCallException() {
	}

	friend std::ostream & operator<<(std::ostream &os, const UnexpectedMethodCallException& val) {
		os << std::string("UnexpectedMethodCallException: could not find any recorded behavior to support this method call");
		return os;
	}

};

enum class VerificationType {
	Exact, AtLeast, NoMoreInvocatoins
};

struct VerificationException: public FakeitException {

	VerificationException() {
	}

	virtual VerificationType verificationType() const = 0;
};

struct NoMoreInvocationsVerificationException: public VerificationException {
	NoMoreInvocationsVerificationException( //
			std::vector<Invocation*>& allIvocations, //
			std::vector<Invocation*>& unverifedIvocations) : //
			VerificationException(), _allIvocations(allIvocations), _unverifedIvocations(unverifedIvocations) { //
	}

	virtual VerificationType verificationType() const override {
		return VerificationType::NoMoreInvocatoins;
	}

	const std::vector<Invocation*>& allIvocations() const {
		return _allIvocations;
	}

	const std::vector<Invocation*>& unverifedIvocations() const {
		return _unverifedIvocations;
	}

	friend std::ostream & operator<<(std::ostream &os, const NoMoreInvocationsVerificationException& val) {
		os << std::string("VerificationException: expected no more invocations but found ") //
		.append(std::to_string(val.unverifedIvocations().size()));
		return os;
	}

private:
	const std::vector<Invocation*> _allIvocations;
	const std::vector<Invocation*> _unverifedIvocations;
};


struct SequenceVerificationException: public VerificationException {
	SequenceVerificationException( //
			std::vector<Sequence*>& expectedPattern, //
			std::vector<Invocation*>& actualSequence, //
			int expectedCount, //
			int actualCount) : //
			VerificationException(), //
			_expectedPattern(expectedPattern), //
			_actualSequence(actualSequence), //
			_expectedCount(expectedCount), //
			_actualCount(actualCount) //
	{ //
	}

	const std::vector<Sequence*>& expectedPattern() const {
		return _expectedPattern;
	}

	const std::vector<Invocation*>& actualSequence() const {
		return _actualSequence;
	}

	const int expectedCount() const {
		return _expectedCount;
	}

	const int actualCount() const {
		return _actualCount;
	}

	friend std::ostream & operator<<(std::ostream &os, const SequenceVerificationException& val) {
		os << std::string("VerificationException: expected ") //
		.append(val.verificationType() == fakeit::VerificationType::Exact ? "exactly " : "at least ") //
		.append(std::to_string(val.expectedCount())) //
		.append(" invocations but was ") //
		.append(std::to_string(val.actualCount()));
		return os;
	}

private:
	const std::vector<Sequence*> _expectedPattern;
	const std::vector<Invocation*> _actualSequence;
	const int _expectedCount;
	const int _actualCount;
};



}
#endif // FakeitExceptions_h__
