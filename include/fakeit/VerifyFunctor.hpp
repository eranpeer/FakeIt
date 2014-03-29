/*
 * VerifyFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: eran
 */

#ifndef VERIFYFUNCTOR_HPP_
#define VERIFYFUNCTOR_HPP_

#include <set>

#include "fakeit/StubbingImpl.h"
#include "fakeit/Stubbing.h"
#include "fakeit/Sequence.hpp"

namespace fakeit {

static void sort(std::unordered_set<AnyInvocation*>& actualIvocations, std::vector<AnyInvocation*>& actualSequence) {
	auto comp = [](AnyInvocation* a, AnyInvocation* b)-> bool {return a->getOrdinal() < b->getOrdinal();};
	std::set<AnyInvocation*, bool (*)(AnyInvocation* a, AnyInvocation* b)> sortedActualIvocations(comp);
	for (auto i : actualIvocations)
		sortedActualIvocations.insert(i);

	for (auto i : sortedActualIvocations)
		actualSequence.push_back(i);
}

class UsingFunctor {

	std::set<ActualInvocationsSource*>& collectMocks(std::set<ActualInvocationsSource*>& into) {
		return into;
	}

	template<typename ... list>
	std::set<ActualInvocationsSource*>& collectMocks(std::set<ActualInvocationsSource*>& into, const ActualInvocationsSource& mock,
			const list&... tail) {
		into.insert(&const_cast<ActualInvocationsSource&>(mock));
		return collectMocks(into, tail...);
	}

public:

	struct VerificationProgress: public virtual MethodVerificationProgress {

		friend class UsingFunctor;
		friend class VerifyFunctor;

		VerificationProgress(VerificationProgress& other) : //
			involvedMocks(other.involvedMocks), //
			expectedPattern(other.expectedPattern), //
			expectedInvocationCount(other.expectedInvocationCount), //
			_isActive(other._isActive) {
			other._isActive = false;
		}

		~VerificationProgress() THROWS {

			if (std::uncaught_exception()) {
				return;
			}

			if (!_isActive) {
				return;
			}

			std::unordered_set<AnyInvocation*> actualIvocations;
			collectActualInvocations(expectedPattern, actualIvocations);

			std::vector<AnyInvocation*> actualSequence;
			sort(actualIvocations, actualSequence);

			std::vector<AnyInvocation*> matchedInvocations;
			int count = countMatches(expectedPattern, actualSequence, matchedInvocations);

			if (expectedInvocationCount < 0) {
				// negative number represents an "AtLeast" search;
				if (count < -expectedInvocationCount) {
					throw VerificationException(
							Mock4cpp.getErrorFromatter().buildAtLeastVerificationErrorMsg(expectedPattern, actualSequence,
									-expectedInvocationCount, count));
				}
			} else if (count != expectedInvocationCount) {
				// "Exact" search.
				throw VerificationException(
						Mock4cpp.getErrorFromatter().buildExactVerificationErrorMsg(expectedPattern, actualSequence,
								expectedInvocationCount, count));
			}

			markAsVerified(matchedInvocations);
		}

		template<typename ... list>
		MethodVerificationProgress& Verify(const Sequence& sequence, const list&... tail) {
			collectSequences(expectedPattern, sequence, tail...);
			return *this;
		}

	private:

		std::set<ActualInvocationsSource*> involvedMocks;
		std::vector<Sequence*> expectedPattern;
		int expectedInvocationCount;
		bool _isActive;

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

		void collectActualInvocations(std::vector<Sequence*>& expectedPattern, std::unordered_set<AnyInvocation*>& actualIvocations) {
			for (auto mock : involvedMocks) {
				mock->getActualInvocations(actualIvocations);
			}
		}
		virtual void verifyInvocations(const int times) override {
			expectedInvocationCount = times;
		}

		void markAsVerified(std::vector<AnyInvocation*>& matchedInvocations) {
			for (auto i : matchedInvocations)
				i->markAsVerified();
		}

		int countMatches(std::vector<Sequence*> &pattern, std::vector<AnyInvocation*>& actualSequence,
				std::vector<AnyInvocation*>& matchedInvocations) {
			int end = -1;
			int count = 0;
			int startSearchIndex = 0;
			while (findNextMatch(pattern, actualSequence, startSearchIndex, end, matchedInvocations)) {
				count++;
				startSearchIndex = end;
			}
			return count;
		}

		void collectMatchedInvocations(std::vector<AnyInvocation*>& actualSequence, std::vector<AnyInvocation*>& matchedInvocations,
				int start, int length) {
			int indexAfterMatchedPattern = start + length;
			for (; start < indexAfterMatchedPattern; start++) {
				matchedInvocations.push_back(actualSequence[start]);
			}
		}

		bool findNextMatch(std::vector<Sequence*> &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex, int& end,
				std::vector<AnyInvocation*>& matchedInvocations) {
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

		bool isMatch(std::vector<AnyInvocation*>& actualSequence, std::vector<AnyInvocation::Matcher*>& expectedSequence, int start) {
			bool found = true;
			for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
				AnyInvocation* actual = actualSequence[start + j];
				AnyInvocation::Matcher* expected = expectedSequence[j];
				found = found && expected->matches(*actual);
			}
			return found;
		}

		int findNextMatch(Sequence* &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex) {
			std::vector<AnyInvocation::Matcher*> expectedSequence;
			pattern->getExpectedSequence(expectedSequence);
			for (int i = startSearchIndex; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
				if (isMatch(actualSequence, expectedSequence, i)) {
					return i;
				}
			}
			return -1;
		}

		VerificationProgress(std::set<ActualInvocationsSource*> mocks) : //
				involvedMocks { mocks }, //
				expectedPattern { }, //
				expectedInvocationCount(-1), //
				_isActive(true) {
		}

	};

public:

	UsingFunctor() {
	}

	template<typename ... list>
	VerificationProgress operator()(const ActualInvocationsSource& mock, const list&... tail) {
		std::set<ActualInvocationsSource*> allMocks;
		collectMocks(allMocks, mock, tail...);
		VerificationProgress progress(allMocks);
		return progress;
	}

}
static Using;

class VerifyFunctor {

	std::vector<Sequence*>& collectSequences(std::vector<Sequence*>& vec) {
		return vec;
	}

	template<typename ... list>
	std::vector<Sequence*>& collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		return collectSequences(vec, tail...);
	}

public:

	VerifyFunctor() {
	}

	template<typename ... list>
	UsingFunctor::VerificationProgress operator()(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> allSequences;
		collectSequences(allSequences, sequence, tail...);

		std::set<ActualInvocationsSource*> allMocks;
		for (auto sequence : allSequences) {
			sequence->getInvolvedMocks(allMocks);
		}

		UsingFunctor::VerificationProgress rv(allMocks);
		rv.Verify(sequence, tail...);
		return rv;
	}

}
static Verify;

class VerifyNoOtherInvocationsFunctor {

	std::string buildNoOtherInvocationsVerificationErrorMsg( //
			std::vector<AnyInvocation*>& allIvocations, //
			std::vector<AnyInvocation*>& unverifedIvocations) {
		auto format = std::string("found ") + std::to_string(unverifedIvocations.size()) + " non verified invocations.\n";
		for (auto invocation : unverifedIvocations) {
			format += invocation->format();
			format += '\n';
		}
		return format;
	}

	template<typename ... list>
	void collectActualInvocations(std::unordered_set<AnyInvocation*>& actualInvocations) {
	}

	template<typename ... list>
	void collectActualInvocations(std::unordered_set<AnyInvocation*>& actualInvocations, const ActualInvocationsSource& head,
			const list&... tail) {
		head.getActualInvocations(actualInvocations);
		collectActualInvocations(actualInvocations, tail...);
	}

	void selectNonVerifiedInvocations(std::unordered_set<AnyInvocation*>& actualInvocations, std::unordered_set<AnyInvocation*>& into) {
		for (auto invocation : actualInvocations) {
			if (!invocation->isVerified()) {
				into.insert(invocation);
			}
		}
	}

public:
	VerifyNoOtherInvocationsFunctor() {
	}

	void operator()() {
	}

	template<typename ... list>
	void operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::unordered_set<AnyInvocation*> actualInvocations;
		collectActualInvocations(actualInvocations, head, tail...);

		std::unordered_set<AnyInvocation*> nonVerifedIvocations;
		selectNonVerifiedInvocations(actualInvocations, nonVerifedIvocations);

		if (nonVerifedIvocations.size() > 0) {
			std::vector<AnyInvocation*> sortedNonVerifedIvocations;
			sort(nonVerifedIvocations, sortedNonVerifedIvocations);

			std::vector<AnyInvocation*> sortedActualIvocations;
			sort(actualInvocations, sortedActualIvocations);

			throw VerificationException(
					Mock4cpp.getErrorFromatter().buildNoOtherInvocationsVerificationErrorMsg(sortedActualIvocations,
							sortedNonVerifedIvocations));
		}
	}
}
static VerifyNoOtherInvocations;

}

#endif /* VERIFYFUNCTOR_HPP_ */
