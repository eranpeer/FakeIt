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

#include "fakeit/FakeitContext.hpp"

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

		FakeitContext& _fakeit;
		std::set<const ActualInvocationsSource*> _mocks;

		std::string _file;
		int _line;
		std::string _callingMethod;

		VerifyNoOtherInvocationsExpectation(FakeitContext& fakeit, std::set<const ActualInvocationsSource*> mocks) :
				_fakeit(fakeit),
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

				NoMoreInvocationsVerificationEvent evt(sortedActualIvocations, sortedNonVerifedIvocations);
				evt.setFileInfo(_file, _line, _callingMethod);
				_fakeit.handle(evt);

				std::string format = _fakeit.format(evt);
				NoMoreInvocationsVerificationException e(format);
				e.setFileInfo(_file, _line, _callingMethod);
				throw e;
			}
		}

	};

	fakeit::smart_ptr<VerifyNoOtherInvocationsExpectation> ptr;

	VerifyNoOtherInvocationsVerificationProgress(VerifyNoOtherInvocationsExpectation * ptr) :
			ptr(ptr) {
	}

	VerifyNoOtherInvocationsVerificationProgress(FakeitContext& fakeit, std::set<const ActualInvocationsSource*>& invocationSources) :
			VerifyNoOtherInvocationsVerificationProgress(new VerifyNoOtherInvocationsExpectation(fakeit, invocationSources)) {
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
