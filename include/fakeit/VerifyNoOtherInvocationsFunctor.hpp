/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef VerifyNoOtherInvocationsFunctor_hpp_
#define VerifyNoOtherInvocationsFunctor_hpp_

#include <set>

#include "fakeit/StubbingImpl.hpp"
#include "fakeit/Stubbing.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/UsingFunctor.hpp"

namespace fakeit {
class VerifyNoOtherInvocationsFunctor {

//	std::string buildNoOtherInvocationsVerificationErrorMsg( //
//			std::vector<AnyInvocation*>& allIvocations, //
//			std::vector<AnyInvocation*>& unverifedIvocations) {
//		auto format = std::string("found ") + std::to_string(unverifedIvocations.size()) + " non verified invocations.\n";
//		for (auto invocation : unverifedIvocations) {
//			format += invocation->format();
//			format += '\n';
//		}
//		return format;
//	}

	template<typename ... list>
	void collectActualInvocations(std::unordered_set<Invocation*>& actualInvocations) {
	}

	template<typename ... list>
	void collectActualInvocations(std::unordered_set<Invocation*>& actualInvocations, const ActualInvocationsSource& head,
			const list&... tail) {
		head.getActualInvocations(actualInvocations);
		collectActualInvocations(actualInvocations, tail...);
	}

	void selectNonVerifiedInvocations(std::unordered_set<Invocation*>& actualInvocations, std::unordered_set<Invocation*>& into) {
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
		std::unordered_set<Invocation*> actualInvocations;
		collectActualInvocations(actualInvocations, head, tail...);

		std::unordered_set<Invocation*> nonVerifedIvocations;
		selectNonVerifiedInvocations(actualInvocations, nonVerifedIvocations);

		if (nonVerifedIvocations.size() > 0) {
			std::vector<Invocation*> sortedNonVerifedIvocations;
			sortByInvocationOrder(nonVerifedIvocations, sortedNonVerifedIvocations);

			std::vector<Invocation*> sortedActualIvocations;
			sortByInvocationOrder(actualInvocations, sortedActualIvocations);

			throw NoMoreInvocationsVerificationException(sortedActualIvocations, sortedNonVerifedIvocations);
		}
	}
}
static VerifyNoOtherInvocations;

}

#endif // VerifyNoOtherInvocationsFunctor_hpp_
