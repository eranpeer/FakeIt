#ifndef SequenceVerificationExpectation_hpp_
#define SequenceVerificationExpectation_hpp_

#include "fakeit/FakeitExceptions.hpp"
#include "mockutils/smart_ptr.hpp"

namespace fakeit {

struct SequenceVerificationExpectation {

	friend class SequenceVerificationProgress;

	~SequenceVerificationExpectation() THROWS{
		if (std::uncaught_exception()) {
			return;
		}
		VerifyExpectation();
	}

	template<typename ... list>
	void setExpectedPattern(const Sequence& sequence, const list&... tail) {
		collectSequences(_expectedPattern, sequence, tail...);
	}

	void setExpectedCount(const int count) {
		_expectedCount = count;
	}

	void setFileInfo(std::string file, int line, std::string callingMethod) {
		_file = file;
		_line = line;
		_testMethod = callingMethod;
	}

private:

	std::set<const ActualInvocationsSource*> _involvedMocks;
	std::vector<Sequence*> _expectedPattern;
	int _expectedCount;

	std::string _file;
	int _line;
	std::string _testMethod;

	SequenceVerificationExpectation(std::set<const ActualInvocationsSource*>& mocks) : //
		_involvedMocks( mocks ), //
		_expectedPattern(), //
		_expectedCount(-1), //
		_line(0) {
	}

	void VerifyExpectation()
	{
		std::unordered_set<Invocation*> actualIvocations;
		collectActualInvocations(_expectedPattern, actualIvocations);

		std::vector<Invocation*> actualSequence;
		sortByInvocationOrder(actualIvocations, actualSequence);

		std::vector<Invocation*> matchedInvocations;
		int count = countMatches(_expectedPattern, actualSequence, matchedInvocations);

		if (isAtLeastVerification() && atLeastLimitNotReached(count)) {
			throwAtLeastVerificationException(actualSequence, count);
		}

		if (isExactVerification() && exactLimitNotMatched(count)) {
			throwExactVerificationException(actualSequence, count);
		}

		markAsVerified(matchedInvocations);
	}

	static inline int AT_LEAST_ONCE() {
		return -1;
	}

	std::vector<Sequence*>& collectSequences(std::vector<Sequence*>& vec) {
		return vec;
	}

	template<typename ... list>
	std::vector<Sequence*>& collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		return collectSequences(vec, tail...);
	}

    void collectActualInvocations(std::vector<Sequence*>& , std::unordered_set<Invocation*>& actualIvocations) {
		for (auto mock : _involvedMocks) {
			mock->getActualInvocations(actualIvocations);
		}
	}

	void markAsVerified(std::vector<Invocation*>& matchedInvocations) {
		for (auto i : matchedInvocations)
			i->markAsVerified();
	}

	int countMatches(std::vector<Sequence*> &pattern, std::vector<Invocation*>& actualSequence,
		std::vector<Invocation*>& matchedInvocations) {
		int end = -1;
		int count = 0;
		int startSearchIndex = 0;
		while (findNextMatch(pattern, actualSequence, startSearchIndex, end, matchedInvocations)) {
			count++;
			startSearchIndex = end;
		}
		return count;
	}

	void collectMatchedInvocations(std::vector<Invocation*>& actualSequence, std::vector<Invocation*>& matchedInvocations, int start,
		int length) {
		int indexAfterMatchedPattern = start + length;
		for (; start < indexAfterMatchedPattern; start++) {
			matchedInvocations.push_back(actualSequence[start]);
		}
	}

	bool findNextMatch(std::vector<Sequence*> &pattern, std::vector<Invocation*>& actualSequence, int startSearchIndex, int& end,
		std::vector<Invocation*>& matchedInvocations) {
		for (auto sequence : pattern) {
			int index = findNextMatch(sequence, actualSequence, startSearchIndex);
			if (index == -1) {
				return false;
			}
			collectMatchedInvocations(actualSequence, matchedInvocations, index, sequence->size());
			startSearchIndex = index + sequence->size();
		}
		end = startSearchIndex;
		return true;
	}

	bool isMatch(std::vector<Invocation*>& actualSequence, std::vector<Invocation::Matcher*>& expectedSequence, int start) {
		bool found = true;
		for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
			Invocation* actual = actualSequence[start + j];
			Invocation::Matcher* expected = expectedSequence[j];
			found = found && expected->matches(*actual);
		}
		return found;
	}

	int findNextMatch(Sequence* &pattern, std::vector<Invocation*>& actualSequence, int startSearchIndex) {
		std::vector<Invocation::Matcher*> expectedSequence;
		pattern->getExpectedSequence(expectedSequence);
		for (int i = startSearchIndex; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
			if (isMatch(actualSequence, expectedSequence, i)) {
				return i;
			}
		}
		return -1;
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

	void throwExactVerificationException(std::vector<Invocation*> actualSequence, int count) {
		struct ExactVerificationException : public SequenceVerificationException {

			ExactVerificationException(std::vector<Sequence*>& expectedPattern, std::vector<Invocation*>& actualSequence,
			int expectedCount, int actualCount) :
			SequenceVerificationException(expectedPattern, actualSequence, expectedCount, actualCount) {
			}

			virtual VerificationType verificationType()const override {
				return VerificationType::Exact;
			}
		};

		ExactVerificationException e(_expectedPattern, actualSequence, _expectedCount, count);
		e.setFileInfo(_file, _line, _testMethod);
		fakeit::FakeIt::handle(e);
		throw e;
	}

	void throwAtLeastVerificationException(std::vector<Invocation*> actualSequence, int count) {
		struct AtLeastVerificationException : public SequenceVerificationException {
			AtLeastVerificationException(std::vector<Sequence*>& expectedPattern, std::vector<Invocation*>& actualSequence,
			int expectedCount, int actualCount) :
			SequenceVerificationException(expectedPattern, actualSequence, expectedCount, actualCount) {
			}

			virtual VerificationType verificationType()const override {
				return VerificationType::AtLeast;
			}
		};

		AtLeastVerificationException e(_expectedPattern, actualSequence, -_expectedCount, count);
		e.setFileInfo(_file, _line, _testMethod);
		FakeIt::handle(e);
		throw e;
	}

};

}

#endif
