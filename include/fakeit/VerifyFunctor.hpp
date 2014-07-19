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
#include "fakeit/Stubbing.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/UsingFunctor.hpp"

namespace fakeit {

class VerifyFunctor {

	void collectSequences(std::vector<Sequence*>& vec) {
	}

	template<typename ... list>
	void collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		collectSequences(vec, tail...);
	}

	template<typename ... list>
	void collectInvolvedMocks(std::set<ActualInvocationsSource*>& invlovedMocks, const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> allSequences;
		collectSequences(allSequences, sequence, tail...);
		for (auto sequence : allSequences) {
			sequence->getInvolvedMocks(invlovedMocks);
		}
	}

public:

	VerifyFunctor() {
	}

	template<typename ... list>
	UsingFunctor::VerificationProgress operator()(const Sequence& sequence, const list&... tail) {
		std::set<ActualInvocationsSource*> invlovedMocks;
		collectInvolvedMocks(invlovedMocks, sequence, tail...);
		UsingFunctor::VerificationProgress progress(invlovedMocks);
		progress.Verify(sequence, tail...);
		return progress;
	}

}
static Verify;

}

#endif // VerifyFunctor_hpp_
