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
#include "fakeit/FakeitContext.hpp"
#include "fakeit/SequenceVerificationProgress.hpp"

#include "mockutils/smart_ptr.hpp"

namespace fakeit {

class UsingFunctor {

	friend class VerifyFunctor;
	FakeitContext& _fakeit;

public:

	UsingFunctor(FakeitContext& fakeit):_fakeit(fakeit) {
	}

	template<typename ... list>
	SequenceVerificationProgress operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::set<const ActualInvocationsSource*> allMocks;
		allMocks.insert(&head);
		collectInvocationSources(allMocks, tail...);
		SequenceVerificationProgress progress (_fakeit, allMocks);
		return progress;
	}

};

}

#endif // UsingFunctor_hpp_
