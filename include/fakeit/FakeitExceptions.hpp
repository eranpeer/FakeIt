#ifndef FakeitExceptions_h__
#define FakeitExceptions_h__

#include <functional>
#include "fakeit/Sequence.hpp"
#include "fakeit/DomainObjects.hpp"

namespace fakeit {

class FakeitException {
};

struct UnexpectedMethodCallException: public FakeitException {
	UnexpectedMethodCallException() {
	}
};

enum class VerificationType {
	Exact, AtLeast, NoMoreInvocatoins
};

struct VerificationException: public FakeitException {

	VerificationException() {
	}

	virtual VerificationType verificationType() = 0;
};

struct NoMoreInvocationsVerificationException: public VerificationException {
	NoMoreInvocationsVerificationException( //
			std::vector<Invocation*>& allIvocations, //
			std::vector<Invocation*>& unverifedIvocations) : //
			VerificationException(), _allIvocations(allIvocations), _unverifedIvocations(unverifedIvocations) { //
	}

	virtual VerificationType verificationType() override {
		return VerificationType::NoMoreInvocatoins;
	}

	const std::vector<Invocation*>& allIvocations() const {
		return _allIvocations;
	}

	const std::vector<Invocation*>& unverifedIvocations() const {
		return _unverifedIvocations;
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

private:
	const std::vector<Sequence*> _expectedPattern;
	const std::vector<Invocation*> _actualSequence;
	const int _expectedCount;
	const int _actualCount;
};

}

#endif // FakeitExceptions_h__
