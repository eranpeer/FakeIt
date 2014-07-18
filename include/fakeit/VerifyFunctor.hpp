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

public:

	VerifyFunctor() {
	}

	template<typename ... list>
	UsingFunctor::VerificationProgressProxy operator()(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> allSequences;
		collectSequences(allSequences, sequence, tail...);
		std::set<ActualInvocationsSource*> allMocks;
		for (auto sequence : allSequences) {
			sequence->getInvolvedMocks(allMocks);
		}

		UsingFunctor::VerificationProgressProxy proxy(new UsingFunctor::VerificationProgress(allMocks));
		proxy.Verify(sequence, tail...);

		return proxy;
	}

}
static Verify;
}

#endif // VerifyFunctor_hpp_
