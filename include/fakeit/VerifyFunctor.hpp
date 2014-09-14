/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef VerifyFunctor_hpp_
#define VerifyFunctor_hpp_

#include <set>
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/SequenceVerificationProgress.hpp"

namespace fakeit {

class VerifyFunctor {

	FakeitContext& _fakeit;

	void collectSequences(std::vector<Sequence*>&) {
	}

	template<typename ... list>
	void collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		collectSequences(vec, tail...);
	}

	void collectInvolvedMocks(std::vector<Sequence*>& allSequences,std::set<const ActualInvocationsSource*>&invlovedMocks){
		for (auto sequence : allSequences) {
			sequence->getInvolvedMocks(invlovedMocks);
		}
	}
public:

	VerifyFunctor(FakeitContext& fakeit):_fakeit(fakeit) {
	}

	template<typename ... list>
	SequenceVerificationProgress operator()(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> allSequences;
		collectSequences(allSequences, sequence, tail...);

		std::set<const ActualInvocationsSource*> invlovedMocks;
		collectInvolvedMocks(allSequences,invlovedMocks);

		UsingProgress usingProgress(_fakeit, invlovedMocks);
		return usingProgress.Verify(sequence, tail... );
	}

};

}

#endif // VerifyFunctor_hpp_
