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
#include "mockutils/smart_ptr.hpp"
#include "mockutils/Macros.hpp"

namespace fakeit {
class VerifyNoOtherInvocationsFunctor {

	template<typename ... list>
	static void collectActualInvocations(std::unordered_set<Invocation*>& actualInvocations,
			std::unordered_set<const ActualInvocationsSource*>& invocationSources) {
		for (auto source : invocationSources) {
			source->getActualInvocations(actualInvocations);
		}
	}

	template<typename ... list>
	static void collectInvocationSources(std::unordered_set<const ActualInvocationsSource*>& invocationSources,
			const ActualInvocationsSource& head, const list&... tail) {
		invocationSources.insert(&head);
		collectInvocationSources(invocationSources, tail...);
	}

	template<typename ... list>
	static void collectInvocationSources(std::unordered_set<const ActualInvocationsSource*>& invocationSources) {
	}

	static void selectNonVerifiedInvocations(std::unordered_set<Invocation*>& actualInvocations, std::unordered_set<Invocation*>& into) {
		for (auto invocation : actualInvocations) {
			if (!invocation->isVerified()) {
				into.insert(invocation);
			}
		}
	}

	struct Expectation {

		friend class VerifyNoOtherInvocationsFunctor;

		~Expectation() THROWS {
			if (std::uncaught_exception()) {
				return;
			}
			VerifyExpectation();
		}

		void setFileInfo(std::string file, int line, std::string callingMethod) {
			_file = file;
			_line = line;
			_callingMethod = callingMethod;
		}

	private:

		std::unordered_set<const ActualInvocationsSource*> _mocks;

		std::string _file;
		int _line;
		std::string _callingMethod;

		Expectation(std::unordered_set<const ActualInvocationsSource*> mocks) :
			_mocks(mocks) {
		}

		void VerifyExpectation()
		{
			std::unordered_set<Invocation*> actualInvocations;
			collectActualInvocations(actualInvocations, _mocks);

			std::unordered_set<Invocation*> nonVerifedIvocations;
			selectNonVerifiedInvocations(actualInvocations, nonVerifedIvocations);

			if (nonVerifedIvocations.size() > 0) {
				std::vector<Invocation*> sortedNonVerifedIvocations;
				sortByInvocationOrder(nonVerifedIvocations, sortedNonVerifedIvocations);

				std::vector<Invocation*> sortedActualIvocations;
				sortByInvocationOrder(actualInvocations, sortedActualIvocations);

				NoMoreInvocationsVerificationException e(sortedActualIvocations, sortedNonVerifedIvocations);
				e.setFileInfo(_file, _line, _callingMethod);
				fakeit::FakeIt::log(e);
				throw e;
			}
		}

	};

public:

	class VerificationProgress {

		friend class VerifyNoOtherInvocationsFunctor;

		fakeit::smart_ptr<Expectation> ptr;

		VerificationProgress(Expectation * ptr) :ptr(ptr){
		}

		VerificationProgress(std::unordered_set<const ActualInvocationsSource*>& invocationSources)
			:VerificationProgress(new Expectation(invocationSources)){
		}

	public:
		~VerificationProgress() THROWS {};		

		VerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
			ptr->setFileInfo(file, line, callingMethod);
			return *this;
		}
	};

	VerifyNoOtherInvocationsFunctor() {
	}

	void operator()() {
	}

	template<typename ... list>
	VerificationProgress operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::unordered_set<const ActualInvocationsSource*> invocationSources;
		collectInvocationSources(invocationSources, head, tail...);
		VerificationProgress proxy{invocationSources};
		return proxy;
	}
}
static VerifyNoOtherInvocations;

}

#endif // VerifyNoOtherInvocationsFunctor_hpp_
