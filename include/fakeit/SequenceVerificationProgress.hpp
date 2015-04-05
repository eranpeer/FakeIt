#ifndef SequenceVerificationProgress_hpp_
#define SequenceVerificationProgress_hpp_

#include <memory>
#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/SequenceVerificationExpectation.hpp"
#include "mockutils/smart_ptr.hpp"
#include "mockutils/to_string.hpp"


namespace fakeit {

	struct FakeitContext;

	class SequenceVerificationProgress {

		friend class UsingFunctor;
		friend class VerifyFunctor;
		friend class UsingProgress;

		smart_ptr<SequenceVerificationExpectation> _expectationPtr;
		//std::shared_ptr<SequenceVerificationExpectation> ptr;

		SequenceVerificationProgress(SequenceVerificationExpectation * ptr) :_expectationPtr(ptr){
		}

		SequenceVerificationProgress(
				FakeitContext& fakeit,
				InvocationsSourceProxy sources,
				std::vector<Sequence*>& allSequences) :
			SequenceVerificationProgress(new SequenceVerificationExpectation(fakeit, sources, allSequences)){
		}

		virtual void verifyInvocations(const int times) {
			_expectationPtr->setExpectedCount(times);
		}

		class Terminator {
			smart_ptr<SequenceVerificationExpectation> _expectationPtr;
		public:
			Terminator(smart_ptr<SequenceVerificationExpectation> expectationPtr):_expectationPtr(expectationPtr){};
			operator bool() {
				try{
					_expectationPtr->VerifyExpectation();
					return true;
				} catch(...){
					return false;
				}
			}
		};

	public:

		~SequenceVerificationProgress() THROWS{};

		operator bool() {
			try{
				_expectationPtr->VerifyExpectation();
				return true;
			} catch(...){
				return false;
			}
		}

		Terminator Never() {
			Exactly(0);
			return Terminator(_expectationPtr);
		}

		Terminator Once() {
			Exactly(1);
			return Terminator(_expectationPtr);
		}

		Terminator Twice() {
			Exactly(2);
			return Terminator(_expectationPtr);
		}

		Terminator AtLeastOnce() {
			verifyInvocations(-1);
			return Terminator(_expectationPtr);
		}

		Terminator Exactly(const int times) {
			if (times < 0) {
				throw std::invalid_argument(std::string("bad argument times:").append(fakeit::to_string(times)));
			}
			verifyInvocations(times);
			return Terminator(_expectationPtr);
		}

		Terminator Exactly(const Quantity & q) {
			Exactly(q.quantity);
			return Terminator(_expectationPtr);
		}

		Terminator AtLeast(const int times) {
			if (times < 0) {
				throw std::invalid_argument(std::string("bad argument times:").append(fakeit::to_string(times)));
			}
			verifyInvocations(-times);
			return Terminator(_expectationPtr);
		}

		Terminator AtLeast(const Quantity & q) {
			AtLeast(q.quantity);
			return Terminator(_expectationPtr);
		}

		SequenceVerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
			_expectationPtr->setFileInfo(file, line, callingMethod);
			return *this;
		}
	};
}

#endif
