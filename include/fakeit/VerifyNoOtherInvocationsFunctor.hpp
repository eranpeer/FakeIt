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

	struct VerificationProgress {

		friend class VerifyNoOtherInvocationsFunctor;

		VerificationProgress(VerificationProgress& other) : //
				_mocks(other._mocks), //
				_file(other._file), //
				_line(other._line), //
				_callingMethod(other._callingMethod) //
		{
		}

		~VerificationProgress() THROWS {
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

		VerificationProgress(std::unordered_set<const ActualInvocationsSource*> mocks) :
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


	class VerificationProgressProxy{
		fakeit::smart_ptr<VerificationProgress> ptr;
	public:
		~VerificationProgressProxy() THROWS {};

		VerificationProgressProxy(VerificationProgress * ptr):ptr(ptr){
		}

		VerificationProgressProxy setFileInfo(std::string file, int line, std::string callingMethod) {
			ptr->setFileInfo(file, line, callingMethod);
			return *this;
		}
	};

	VerifyNoOtherInvocationsFunctor() {
	}

	void operator()() {
	}

	template<typename ... list>
	VerificationProgressProxy operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::unordered_set<const ActualInvocationsSource*> invocationSources;
		collectInvocationSources(invocationSources, head, tail...);
		VerificationProgressProxy proxy{new VerificationProgress(invocationSources)};
		return proxy;
	}
}
static VerifyNoOtherInvocations;

}

#endif // VerifyNoOtherInvocationsFunctor_hpp_
