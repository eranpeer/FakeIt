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
				std::set<const ActualInvocationsSource*>& sources,
				std::vector<Sequence*>& allSequences) :
			SequenceVerificationProgress(new SequenceVerificationExpectation(fakeit, sources, allSequences)){
		}

		virtual void verifyInvocations(const int times) {
			_expectationPtr->setExpectedCount(times);
		}

	public:

		~SequenceVerificationProgress() THROWS{};

		void Never() {
			Exactly(0);
		}

		void Once() {
			Exactly(1);
		}

		void Twice() {
			Exactly(2);
		}

		void AtLeastOnce() {
			verifyInvocations(-1);
		}

		void Exactly(const int times) {
			if (times < 0) {
				throw std::invalid_argument(std::string("bad argument times:").append(fakeit::to_string(times)));
			}
			verifyInvocations(times);
		}

		void Exactly(const Quantity & q) {
			Exactly(q.quantity);
		}

		void AtLeast(const int times) {
			if (times < 0) {
				throw std::invalid_argument(std::string("bad argument times:").append(fakeit::to_string(times)));
			}
			verifyInvocations(-times);
		}

		void AtLeast(const Quantity & q) {
			AtLeast(q.quantity);
		}

		SequenceVerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
			_expectationPtr->setFileInfo(file, line, callingMethod);
			return *this;
		}
	};
}

#endif
