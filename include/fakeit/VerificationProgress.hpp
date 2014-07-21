#ifndef VerificationProgress_hpp_
#define VerificationProgress_hpp_

#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/Expectation.hpp"
#include "mockutils/smart_ptr.hpp"

namespace fakeit {

	class VerificationProgress {

		friend class UsingFunctor;
		friend class VerifyFunctor;

		fakeit::smart_ptr<Expectation> ptr;

		VerificationProgress(Expectation * ptr) :ptr(ptr){
		}

		VerificationProgress(std::set<ActualInvocationsSource*>& sources) :VerificationProgress(new Expectation(sources)){
		}

		virtual void verifyInvocations(const int times) {
			ptr->setExpectedCount(times);
		}

	public:

		virtual ~VerificationProgress() THROWS{};

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

		VerificationProgress setFileInfo(std::string file, int line, std::string callingMethod) {
			ptr->setFileInfo(file, line, callingMethod);
			return *this;
		}

		template<typename ... list>
		VerificationProgress Verify(const Sequence& sequence, const list&... tail) {
			ptr->setExpectedPattern(sequence, tail...);
			return *this;
		}
	};
}

#endif
