/*
 * VerifyNoOtherInvocationsVerificationProgress.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jul 21, 2014
 */
#ifndef VerifyNoOtherInvocationsVerificationProgress_hpp_
#define VerifyNoOtherInvocationsVerificationProgress_hpp_

namespace fakeit {

class VerifyNoOtherInvocationsVerificationProgress {

	friend class VerifyNoOtherInvocationsFunctor;

	struct VerifyNoOtherInvocationsExpectation {

		friend class VerifyNoOtherInvocationsVerificationProgress;

		~VerifyNoOtherInvocationsExpectation() THROWS {
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

		std::set<const ActualInvocationsSource*> _mocks;

		std::string _file;
		int _line;
		std::string _callingMethod;

		VerifyNoOtherInvocationsExpectation(std::set<const ActualInvocationsSource*> mocks) :
				_mocks(mocks), _line(0) {
		}

		void VerifyExpectation() {
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
				fakeit::FakeIt::getInstance().handle(e);
				throw e;
			}
		}

	};

	fakeit::smart_ptr<VerifyNoOtherInvocationsExpectation> ptr;

	VerifyNoOtherInvocationsVerificationProgress(VerifyNoOtherInvocationsExpectation * ptr) :
			ptr(ptr) {
	}

	VerifyNoOtherInvocationsVerificationProgress(std::set<const ActualInvocationsSource*>& invocationSources) :
			VerifyNoOtherInvocationsVerificationProgress(new VerifyNoOtherInvocationsExpectation(invocationSources)) {
	}

public:
	~VerifyNoOtherInvocationsVerificationProgress() THROWS {};

	VerifyNoOtherInvocationsVerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
		ptr->setFileInfo(file, line, callingMethod);
		return *this;
	}
};

}

#endif
