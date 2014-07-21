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
#include "fakeit/FakeIt.hpp"
#include "fakeit/VerificationProgress.hpp"

#include "mockutils/smart_ptr.hpp"
#include "VerificationProgress.hpp"

namespace fakeit {

class UsingFunctor {

	friend class VerifyFunctor;

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

	UsingFunctor() {
	}

	template<typename ... list>
	VerificationProgress operator()(const ActualInvocationsSource& mock, const list&... tail) {
		std::set<ActualInvocationsSource*> allMocks;
		collectMocks(allMocks, mock, tail...);
		VerificationProgress progress (allMocks);
		return progress;
	}

}
static Using;

}

#endif // UsingFunctor_hpp_
