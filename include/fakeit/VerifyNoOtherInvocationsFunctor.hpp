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

#include "fakeit/StubbingImpl.hpp"
#include "fakeit/Stubbing.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/SortInvocations.hpp"
#include "fakeit/UsingFunctor.hpp"

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

public:

	struct VerificationProgress {

		friend class VerifyNoOtherInvocationsFunctor;

		VerificationProgress(VerificationProgress& other) : //
				_mocks(other._mocks), //
				_isActive(other._isActive), //
				_file(other._file), //
				_line(other._line), //
				_testMethod(other._testMethod) //
		{
			other._isActive = false;
		}

		~VerificationProgress() THROWS {

			if (std::uncaught_exception()) {
				_isActive = false;
			}

			if (!_isActive) {
				return;
			}

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
				e.setFileInfo(_file,_line,_testMethod);
				fakeit::FakeIt::log(e);
				throw e;
			}
		}

		VerificationProgress& setFileInfo(std::string file, int line, std::string testMethod) {
			_file = file;
			_line = line;
			_testMethod = testMethod;
			return *this;
		}

	private:

		std::unordered_set<const ActualInvocationsSource*> _mocks;
		bool _isActive;

		std::string _file;
		int _line;
		std::string _testMethod;

		VerificationProgress(std::unordered_set<const ActualInvocationsSource*> mocks) :
				_mocks(mocks), //
				_isActive(true) {
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
		VerificationProgress progress(invocationSources);
		return progress;
	}
}
static VerifyNoOtherInvocations;

}

#endif // VerifyNoOtherInvocationsFunctor_hpp_
