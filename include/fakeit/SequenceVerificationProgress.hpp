#ifndef SequenceVerificationProgress_hpp_
#define SequenceVerificationProgress_hpp_

#include <memory>
#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/SequenceVerificationExpectation.hpp"
#include "mockutils/smart_ptr.hpp"


namespace fakeit {

	struct FakeitContext;

	class SequenceVerificationProgress {

		friend class UsingFunctor;
		friend class VerifyFunctor;
		friend class UsingProgress;

		smart_ptr<SequenceVerificationExpectation> ptr;
		//std::shared_ptr<SequenceVerificationExpectation> ptr;

		SequenceVerificationProgress(SequenceVerificationExpectation * ptr) :ptr(ptr){
		}

		SequenceVerificationProgress(
				FakeitContext& fakeit,
				std::set<const ActualInvocationsSource*>& sources,
				std::vector<Sequence*>& allSequences) :
			SequenceVerificationProgress(new SequenceVerificationExpectation(fakeit, sources, allSequences)){
		}

		virtual void verifyInvocations(const int times) {
			ptr->setExpectedCount(times);
		}

	public:

		virtual ~SequenceVerificationProgress() THROWS{};

		void Never() {
			Exactly(0);
		}

		virtual void Once() {
			Exactly(1);
		}

		virtual void Twice() {
			Exactly(2);
		}

		virtual void AtLeastOnce() {
			verifyInvocations(-1);
		}

		virtual void Exactly(const int times) {
			if (times < 0) {
				throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
			}
			verifyInvocations(times);
		}

		virtual void Exactly(const Quantity & q) {
			Exactly(q.quantity);
		}

		virtual void AtLeast(const int times) {
			if (times < 0) {
				throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
			}
			verifyInvocations(-times);
		}

		virtual void AtLeast(const Quantity & q) {
			AtLeast(q.quantity);
		}

		SequenceVerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
			ptr->setFileInfo(file, line, callingMethod);
			return *this;
		}

//		template<typename ... list>
//		SequenceVerificationProgress Verify(const Sequence& sequence, const list&... tail) {
//			ptr->setExpectedPattern(sequence, tail...);
//			return *this;
//		}
	};
}

#endif
