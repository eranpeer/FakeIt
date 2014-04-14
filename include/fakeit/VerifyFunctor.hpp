/*
 * VerifyFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: Eran
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

	std::vector<Sequence*>& collectSequences(std::vector<Sequence*>& vec) {
		return vec;
	}

	template<typename ... list>
	std::vector<Sequence*>& collectSequences(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		return collectSequences(vec, tail...);
	}

public:

	VerifyFunctor() {
	}

	template<typename ... list>
	UsingFunctor::VerificationProgress operator()(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> allSequences;
		collectSequences(allSequences, sequence, tail...);

		std::set<ActualInvocationsSource*> allMocks;
		for (auto sequence : allSequences) {
			sequence->getInvolvedMocks(allMocks);
		}

		UsingFunctor::VerificationProgress rv(allMocks);
		rv.Verify(sequence, tail...);
		return rv;
	}

}
static Verify;
}

#endif // VerifyFunctor_hpp_
