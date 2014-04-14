/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef UsingFunctor_hpp_
#define UsingFunctor_hpp_

#include <set>

#include "fakeit/StubbingImpl.hpp"
#include "fakeit/Stubbing.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"

namespace fakeit {

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

		~VerificationProgress() THROWS {

			if (std::uncaught_exception()) {
				_isActive = false;
			}

			if (!_isActive) {
				return;
			}

			std::unordered_set<Invocation*> actualIvocations;
			collectActualInvocations(expectedPattern, actualIvocations);

			std::vector<Invocation*> actualSequence;
			sortByInvocationOrder(actualIvocations, actualSequence);

			std::vector<Invocation*> matchedInvocations;
			int count = countMatches(expectedPattern, actualSequence, matchedInvocations);

			if (expectedInvocationCount < 0) {
				struct AtLeastVerificationException: public SequenceVerificationException {
					AtLeastVerificationException(std::vector<Sequence*>& expectedPattern, std::vector<Invocation*>& actualSequence, int expectedCount,
							int actualCount) :
							SequenceVerificationException(expectedPattern, actualSequence, expectedCount, actualCount) {
					}

					virtual VerificationType verificationType() override {
						return VerificationType::AtLeast;
					}
				};

				// negative number represents an "AtLeast" search;
				if (count < -expectedInvocationCount) {
					throw AtLeastVerificationException(expectedPattern, actualSequence, -expectedInvocationCount, count);
				}
			} else if (count != expectedInvocationCount) {
				struct ExactVerificationException: public SequenceVerificationException {
					ExactVerificationException(std::vector<Sequence*>& expectedPattern, std::vector<Invocation*>& actualSequence, int expectedCount,
							int actualCount) :
							SequenceVerificationException(expectedPattern, actualSequence, expectedCount, actualCount) {
					}

					virtual VerificationType verificationType() override {
						return VerificationType::Exact;
					}

				};

				// "Exact" search.
				throw ExactVerificationException(expectedPattern, actualSequence, expectedInvocationCount, count);
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
		int expectedInvocationCount;bool _isActive;

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

		void collectActualInvocations(std::vector<Sequence*>& expectedPattern, std::unordered_set<Invocation*>& actualIvocations) {
			for (auto mock : involvedMocks) {
				mock->getActualInvocations(actualIvocations);
			}
		}
		virtual void verifyInvocations(const int times) override {
			expectedInvocationCount = times;
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

		void collectMatchedInvocations(std::vector<Invocation*>& actualSequence, std::vector<Invocation*>& matchedInvocations,
				int start, int length) {
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

		VerificationProgress(std::set<ActualInvocationsSource*> mocks) : //
				involvedMocks { mocks }, //
				expectedPattern { }, //
				expectedInvocationCount(-1), //
				_isActive(true) {
		}

		VerificationProgress(VerificationProgress& other) : //
				involvedMocks(other.involvedMocks), //
				expectedPattern(other.expectedPattern), //
				expectedInvocationCount(other.expectedInvocationCount), //
				_isActive(other._isActive) {
			other._isActive = false;
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

}

#endif // UsingFunctor_hpp_
