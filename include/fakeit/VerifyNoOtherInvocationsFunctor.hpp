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
#include <memory>

#include "fakeit/StubbingImpl.hpp"
#include "fakeit/Stubbing.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsVerificationProgress.hpp"

#include "mockutils/smart_ptr.hpp"
#include "mockutils/Macros.hpp"
#include "fakeit/FakeIt.hpp"

namespace fakeit {
class VerifyNoOtherInvocationsFunctor {

public:

	VerifyNoOtherInvocationsFunctor() {
		FakeIt::initIfNeeded();
	}

	void operator()() {
	}

	template<typename ... list>
	VerifyNoOtherInvocationsVerificationProgress operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::set<const ActualInvocationsSource*> invocationSources;
		invocationSources.insert(&head);
		collectInvocationSources(invocationSources, tail...);
		VerifyNoOtherInvocationsVerificationProgress progress{invocationSources};
		return progress;
	}
}
static VerifyNoOtherInvocations;

}

#endif // VerifyNoOtherInvocationsFunctor_hpp_
