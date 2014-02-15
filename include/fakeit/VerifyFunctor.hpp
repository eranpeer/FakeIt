/*
 * VerifyFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: eran
 */

#ifndef VERIFYFUNCTOR_HPP_
#define VERIFYFUNCTOR_HPP_

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

class VerifyFunctor {

	std::vector<Sequence*>& concat(std::vector<Sequence*>& vec) {
		return vec;
	}

	template<typename ... list>
	std::vector<Sequence*>& concat(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		return concat(vec, tail...);
	}

public:

	struct VerificationProgress: public virtual MethodVerificationProgress {

		friend class VerifyFunctor;

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
				if (count < -expectedInvocationCount) {
					throw VerificationException(
							Mock4cpp.getErrorFromatter().buildAtLeastVerificationErrorMsg(expectedPattern, actualSequence,
									-expectedInvocationCount, count));
				}
			} else if (count != expectedInvocationCount) {
				throw VerificationException(
						Mock4cpp.getErrorFromatter().buildExactVerificationErrorMsg(expectedPattern, actualSequence,
								expectedInvocationCount, count));
			}

			markAsVerified(matchedInvocations);
		}

	private:

		std::vector<Sequence*> expectedPattern;
		const Sequence& sequence;
		int expectedInvocationCount;bool _isActive;

		static inline int AT_LEAST_ONCE() {
			return -1;
		}

		VerificationProgress(const Sequence& sequence) :
				sequence(sequence), expectedInvocationCount(-1), _isActive(true) {
			expectedPattern.push_back(&const_cast<Sequence&>(sequence));
		}

		VerificationProgress(const std::vector<Sequence*> expectedPattern) :
				expectedPattern(expectedPattern), sequence(*expectedPattern[0]), expectedInvocationCount(-1), _isActive(true) {
		}

		VerificationProgress(VerificationProgress& other) :
				expectedPattern(other.expectedPattern), sequence(other.sequence), expectedInvocationCount(other.expectedInvocationCount), _isActive(
						other._isActive) {
			other._isActive = false;
		}

		void collectActualInvocations(std::vector<Sequence*>& expectedPattern, std::unordered_set<AnyInvocation*>& actualIvocations) {
			for (auto scenario : expectedPattern) {
				scenario->getActualInvocations(actualIvocations);
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

		bool isMatch(std::vector<AnyInvocation*>& actualSequence, std::vector<AnyInvocationMatcher*>& expectedSequence, int start) {
			bool found = true;
			for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
				AnyInvocation* actual = actualSequence[start + j];
				AnyInvocationMatcher* expected = expectedSequence[j];
				if (j >= 1) {
					AnyInvocation* prevActual = actualSequence[start + j - 1];
					found = actual->getOrdinal() - prevActual->getOrdinal() == 1;
				}
				found = found && expected->matches(*actual);
			}
			return found;
		}

		int findNextMatch(Sequence* &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex) {
			std::vector<AnyInvocationMatcher*> expectedSequence;
			pattern->getExpectedSequence(expectedSequence);
			for (int i = startSearchIndex; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
				if (isMatch(actualSequence, expectedSequence, i)) {
					return i;
				}
			}
			return -1;
		}

	};

	VerifyFunctor() {
	}

	template<typename ... list>
	VerificationProgress operator()(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> vec;
		concat(vec, sequence, tail...);
		VerificationProgress progress(vec);
		return progress;
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
