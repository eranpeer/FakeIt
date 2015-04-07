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
		std::set<ActualInvocationsSource*> _mocks;

		std::string _file;
		int _line;
		std::string _callingMethod;
		bool _isVerified;
		VerifyNoOtherInvocationsExpectation(FakeitContext& fakeit, std::set<ActualInvocationsSource*> mocks) :
				_fakeit(fakeit),
				_mocks(mocks), 
				_line(0),
				_isVerified(false){
		}

		VerifyNoOtherInvocationsExpectation(VerifyNoOtherInvocationsExpectation& other) = default;

		void VerifyExpectation() {
			if (_isVerified)
				return;
			_isVerified = true;

			std::unordered_set<Invocation*> actualInvocations;
            InvocationUtils::collectActualInvocations(actualInvocations, _mocks);

			std::unordered_set<Invocation*> nonVerifedIvocations;
            InvocationUtils::selectNonVerifiedInvocations(actualInvocations, nonVerifedIvocations);

			if (nonVerifedIvocations.size() > 0) {
				std::vector<Invocation*> sortedNonVerifedIvocations;
                InvocationUtils::sortByInvocationOrder(nonVerifedIvocations, sortedNonVerifedIvocations);

				std::vector<Invocation*> sortedActualIvocations;
                InvocationUtils::sortByInvocationOrder(actualInvocations, sortedActualIvocations);

				NoMoreInvocationsVerificationEvent evt(sortedActualIvocations, sortedNonVerifedIvocations);
				evt.setFileInfo(_file, _line, _callingMethod);
				_fakeit.handle(evt);
			}
		}

	};

	fakeit::smart_ptr<VerifyNoOtherInvocationsExpectation> _ptr;

	VerifyNoOtherInvocationsVerificationProgress(VerifyNoOtherInvocationsExpectation * ptr) :
			_ptr(ptr) {
	}

	VerifyNoOtherInvocationsVerificationProgress(FakeitContext& fakeit, std::set<ActualInvocationsSource*>& invocationSources)
		: VerifyNoOtherInvocationsVerificationProgress(
			new VerifyNoOtherInvocationsExpectation(fakeit, invocationSources)
			) 
	{
	}

	bool toBool()  {
		try{
			_ptr->VerifyExpectation();
			return true;
		}
		catch (...){
			return false;
		}
	}

public:


	~VerifyNoOtherInvocationsVerificationProgress() THROWS {
	};

	VerifyNoOtherInvocationsVerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
		_ptr->setFileInfo(file, line, callingMethod);
		return *this;
	}

	operator bool() {
		return toBool();
	}

	bool operator ! () const { return !const_cast<VerifyNoOtherInvocationsVerificationProgress*>(this)->toBool(); }

};

}

#endif
