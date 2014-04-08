#ifndef FakeitExceptions_h__
#define FakeitExceptions_h__

#include <functional>
#include "fakeit/Sequence.hpp"
#include "fakeit/DomainObjects.hpp"

namespace fakeit {

class FakeitException {
	//virtual const std::string what() const throw () = 0;
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
			std::vector<AnyInvocation*>& allIvocations, //
			std::vector<AnyInvocation*>& unverifedIvocations) : //
			VerificationException(), _allIvocations(allIvocations), _unverifedIvocations(unverifedIvocations) { //
	}

	virtual VerificationType verificationType() override {
		return VerificationType::NoMoreInvocatoins;
	}

	const std::vector<AnyInvocation*>& allIvocations() const {
		return _allIvocations;
	}

	const std::vector<AnyInvocation*>& unverifedIvocations() const {
		return _unverifedIvocations;
	}

private:
	const std::vector<AnyInvocation*> _allIvocations;
	const std::vector<AnyInvocation*> _unverifedIvocations;
};

struct SequenceVerificationException: public VerificationException {
	SequenceVerificationException( //
			std::vector<Sequence*>& expectedPattern, //
			std::vector<AnyInvocation*>& actualSequence, //
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

	const std::vector<AnyInvocation*>& actualSequence() const {
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
	const std::vector<AnyInvocation*> _actualSequence;
	const int _expectedCount;
	const int _actualCount;
};

struct ExactVerificationException: public SequenceVerificationException {
	ExactVerificationException(std::vector<Sequence*>& expectedPattern, std::vector<AnyInvocation*>& actualSequence, int expectedCount,
			int actualCount) :
			SequenceVerificationException(expectedPattern, actualSequence, expectedCount, actualCount) {
	}

	virtual VerificationType verificationType() override {
		return VerificationType::Exact;
	}

};

struct AtLeastVerificationException: public SequenceVerificationException {
	AtLeastVerificationException(std::vector<Sequence*>& expectedPattern, std::vector<AnyInvocation*>& actualSequence, int expectedCount,
			int actualCount) :
			SequenceVerificationException(expectedPattern, actualSequence, expectedCount, actualCount) {
	}

	virtual VerificationType verificationType() override {
		return VerificationType::AtLeast;
	}
};

}

#endif // FakeitExceptions_h__
