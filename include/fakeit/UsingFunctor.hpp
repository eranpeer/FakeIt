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
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/SequenceVerificationProgress.hpp"

#include "mockutils/smart_ptr.hpp"

namespace fakeit {

class UsingProgress {
	FakeitContext& _fakeit;
	std::set<const ActualInvocationsSource*> _sources;

	void collectSequences(std::vector<Sequence*>&) {
	}

	template<typename ... list>
	void collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		collectSequences(vec, tail...);
	}

public:

	UsingProgress(FakeitContext& fakeit, std::set<const ActualInvocationsSource*>& sources) :
		_fakeit(fakeit),
		_sources(sources) {
	}

	template<typename ... list>
	SequenceVerificationProgress Verify(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> allSequences;
		collectSequences(allSequences, sequence, tail...);
		SequenceVerificationProgress progress(_fakeit, _sources, allSequences);
		return progress;
	}
};

class UsingFunctor {

	friend class VerifyFunctor;
	FakeitContext& _fakeit;

public:

	UsingFunctor(FakeitContext& fakeit):_fakeit(fakeit) {
	}

	template<typename ... list>
	UsingProgress operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::set<const ActualInvocationsSource*> allMocks;
		allMocks.insert(&head);
		collectInvocationSources(allMocks, tail...);
		UsingProgress progress (_fakeit, allMocks);
		return progress;
	}

};

}

#endif // UsingFunctor_hpp_
