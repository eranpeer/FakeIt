#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>

#include "mock4cpp/MethodMock.h"
#include "mock4cpp/Stubbing.h"
#include "mock4cpp/ActualInvocation.h"
#include "mock4cpp/InvocationMatcher.h"
#include "mockutils/ExtractMemberType.h"

namespace mock4cpp {

enum class ProgressType {
	NONE, STUBBING, VERIFYING
};

template<typename R, typename ... arglist>
struct StubbingContext {
	virtual ~StubbingContext() {
	}
	virtual MethodMock<R, arglist...>& getMethodMock() = 0;
};

template<typename R, typename ... arglist>
struct FunctionStubbingProgress: protected virtual FirstFunctionStubbingProgress<R, arglist...>, //
		protected virtual NextFunctionStubbingProgress<R, arglist...> {

	FunctionStubbingProgress() = default;
	virtual ~FunctionStubbingProgress() override = default;

	NextFunctionStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		recordedMethodBody().appendDo(method);
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		recordedMethodBody().clear();
		recordedMethodBody().appendDo(method);
		return *this;
	}

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() = 0;
private:
	FunctionStubbingProgress & operator=(const FunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct ProcedureStubbingProgress: protected virtual FirstProcedureStubbingProgress<R, arglist...>, //
		protected virtual NextProcedureStubbingProgress<R, arglist...> {

	ProcedureStubbingProgress() = default;
	~ProcedureStubbingProgress() override = default;

	NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		recordedMethodBody().appendDo(method);
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		recordedMethodBody().clear();
		recordedMethodBody().appendDo(method);
		return *this;
	}

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() = 0;
private:
	ProcedureStubbingProgress & operator=(const ProcedureStubbingProgress & other) = delete;
};

using namespace mock4cpp;

template<typename R, typename ... arglist>
class MethodStubbingBase: //
protected virtual MethodStubbingInternal,
		protected virtual MethodVerificationProgress,
		public virtual Sequence,
		protected virtual AnyInvocationMatcher {
private:

	std::shared_ptr<RecordedMethodBody<R, arglist...>> buildInitialMethodBody() {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody { new RecordedMethodBody<R, arglist...>() };
		recordedMethodBody->appendDo(initialMethodBehavior);
		return recordedMethodBody;
	}

protected:
	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;
	friend ConcatenatedSequence operator+(const Sequence &s1, const Sequence &s2);
	friend RepeatedSequence operator*(const Sequence &s1, const int times);

	std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext;
	std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher;
	std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody;

	ProgressType progressType;
	int expectedInvocationCount;

	MethodStubbingBase(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			stubbingContext(stubbingContext), invocationMatcher { new DefaultInvocationMatcher<arglist...>() }, progressType(
					ProgressType::NONE), expectedInvocationCount(-1) {
		recordedMethodBody = buildInitialMethodBody();
	}

	int CountInvocations(InvocationMatcher<arglist...>& invocationMatcher) {
		int times = stubbingContext->getMethodMock().getActualInvocations(invocationMatcher).size();
		return times;
	}

	void setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = invocationMatcher;
	}

	virtual ~MethodStubbingBase() THROWS {
		if (progressType == ProgressType::NONE) {
			return;
		}

		if (progressType == ProgressType::STUBBING) {
			stubbingContext->getMethodMock().stubMethodInvocation(invocationMatcher, recordedMethodBody);
			return;
		}

		if (Sequence::isActive()) {
			auto actualInvocations = CountInvocations(*invocationMatcher);
			if (expectedInvocationCount == -1) {
				if (actualInvocations == 0) {
					if (!std::uncaught_exception()) {
						throw(MethodCallVerificationException(std::string("no matching invocation")));
					}
				}
				return;
			}
			if (actualInvocations != expectedInvocationCount) {
				if (!std::uncaught_exception()) {
					throw(MethodCallVerificationException(
							std::string("expected ") + std::to_string(expectedInvocationCount) + " but was "
									+ std::to_string(actualInvocations)));
				}
			}
		}
	}

	virtual void cancelVerification() {
		progressType = ProgressType::NONE;
		Sequence::cancelVerification();
	}

	virtual void startStubbing() {
		progressType = ProgressType::STUBBING;
	}

	virtual void startVerification() override {
		progressType = ProgressType::VERIFYING;
		Sequence::startVerification();
	}

	virtual void verifyInvocations(const int times) override {
		startVerification();
		expectedInvocationCount = times;
	}

public:
	virtual bool matches(AnyInvocation& invocation) override {
		if (&invocation.getMethod() != &stubbingContext->getMethodMock()) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return invocationMatcher->matches(actualInvocation);
	}

	void getActualInvocationSequence(std::unordered_set<AnyInvocation*>& into) const override {
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>>actualInvocations = stubbingContext->getMethodMock().getActualInvocations(*invocationMatcher);
		for (auto i : actualInvocations) {
			AnyInvocation* ai = i.get();
			into.insert(ai);
		}
	}

	void getExpectedInvocationSequence(std::vector<AnyInvocationMatcher*>& into) const override {
		const AnyInvocationMatcher* b = this;
		AnyInvocationMatcher* c = const_cast<AnyInvocationMatcher*>(b);
		into.push_back(c);
	}

};

template<typename R, typename ... arglist>
class FunctionStubbingRoot: //
public virtual MethodStubbingBase<R, arglist...>, //
		private virtual FunctionStubbingProgress<R, arglist...> {
private:
	FunctionStubbingRoot & operator=(const FunctionStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;
	friend ConcatenatedSequence operator+(const Sequence &s1, const Sequence &s2);
	friend RepeatedSequence operator*(const Sequence &s1, const int times);

protected:

	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() override {
		return *MethodStubbingBase<R, arglist...>::recordedMethodBody;
	}

	virtual void startStubbing() override {
		MethodStubbingBase<R, arglist...>::startStubbing();
	}

	virtual void verifyInvocations(const int times) override {
		MethodStubbingBase<R, arglist...>::verifyInvocations(times);
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void startVerification() override {
		MethodStubbingBase<R, arglist...>::startVerification();
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void cancelVerification() override {
		MethodStubbingBase<R, arglist...>::cancelVerification();
	}

public:

	FunctionStubbingRoot(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			MethodStubbingBase<R, arglist...>(stubbingContext), FirstFunctionStubbingProgress<R, arglist...>(), FunctionStubbingProgress<R,
					arglist...>() {
	}

	FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

	~FunctionStubbingRoot() THROWS {
	}

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstFunctionStubbingProgress<R, arglist...>::operator =(method);
	}

	FunctionStubbingRoot<R, arglist...>& Using(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<R, arglist...>::startStubbing();
		return FunctionStubbingProgress<R, arglist...>::Do(method);
	}
};

template<typename R, typename ... arglist>
class ProcedureStubbingRoot: //
public virtual MethodStubbingBase<R, arglist...>,
		private virtual ProcedureStubbingProgress<R, arglist...> {
private:
	ProcedureStubbingRoot & operator=(const ProcedureStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;
	friend ConcatenatedSequence operator+(const Sequence &s1, const Sequence &s2);
	friend RepeatedSequence operator*(const Sequence &s1, const int times);

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() override {
		return *MethodStubbingBase<R, arglist...>::recordedMethodBody;
	}

	virtual void startStubbing() override {
		MethodStubbingBase<R, arglist...>::startStubbing();
	}

	virtual void verifyInvocations(const int times) override {
		MethodStubbingBase<R, arglist...>::verifyInvocations(times);
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void startVerification() override {
		MethodStubbingBase<R, arglist...>::startVerification();
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void cancelVerification() override {
		MethodStubbingBase<R, arglist...>::cancelVerification();
	}

public:
	ProcedureStubbingRoot(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			MethodStubbingBase<R, arglist...>(stubbingContext), FirstProcedureStubbingProgress<R, arglist...>(), ProcedureStubbingProgress<
					R, arglist...>() {
	}

	~ProcedureStubbingRoot() THROWS {
	}

	ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstProcedureStubbingProgress<R, arglist...>::operator =(method);
	}

	ProcedureStubbingRoot<R, arglist...>& Using(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<R, arglist...>::startStubbing();
		return ProcedureStubbingProgress<R, arglist...>::Do(method);
	}
};

template<typename C, typename DATA_TYPE>
class DataMemberStubbingRoot {
private:
	//DataMemberStubbingRoot & operator= (const DataMemberStubbingRoot & other) = delete;
public:
	DataMemberStubbingRoot(const DataMemberStubbingRoot& other) = default;
	DataMemberStubbingRoot() = default;

	void operator=(const DATA_TYPE& val) {
	}
};

class VerifyFunctor {
public:

	struct VerificationProgress: public virtual MethodVerificationProgress {
		VerificationProgress(const Sequence& sequence) :
				sequence(sequence), expectedInvocationCount(-1), _isActive(true) {
		}

		~VerificationProgress() THROWS {

			if (!_isActive)
				return;

			if (std::uncaught_exception()) {
				return;
			}

			std::unordered_set<AnyInvocation*> actualIvocations;
			sequence.getActualInvocationSequence(actualIvocations);

			auto comp = [](AnyInvocation* a, AnyInvocation* b)-> bool {return a->getOrdinal() < b->getOrdinal();};
			std::set<AnyInvocation*, decltype(comp)> sortedActualIvocations(comp);
			for (auto i : actualIvocations)
				sortedActualIvocations.insert(i);

			std::vector<AnyInvocation*> actualSequence;
			for (auto i : sortedActualIvocations)
				actualSequence.push_back(i);

			std::vector<AnyInvocationMatcher*> expectedSequence;
			sequence.getExpectedInvocationSequence(expectedSequence);

			int count = 0;
			for (int i = 0; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
				bool found = true;
				for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
					AnyInvocation* actual = actualSequence[i + j];
					AnyInvocationMatcher* expected = expectedSequence[j];
					if (j >= 1) {
						AnyInvocation* prevActual = actualSequence[i + j - 1];
						found = actual->getOrdinal() - prevActual->getOrdinal() == 1;
					}
					found = found || expected->matches(*actual);
				}
				if (found) {
					count++;
					i += (expectedSequence.size() - 1);
				}
			}
			if (expectedInvocationCount == -1) {
				if (count == 0) {
					throw(MethodCallVerificationException(
							std::string("Expected invocation scenario does not match actual invocation order")));
				}
				return;
			}
			if (count != expectedInvocationCount) {
				throw(MethodCallVerificationException(
						std::string("expected ") + std::to_string(expectedInvocationCount) + " but was " + std::to_string(count)));
			}
		}

		virtual void cancelVerification() {
			_isActive = false;
		}

		virtual void startVerification() override {
		}

		virtual void verifyInvocations(const int times) override {
			expectedInvocationCount = times;
		}

	private:
		const Sequence& sequence;
		int expectedInvocationCount;
		bool _isActive; // not needed since we chech for uncought exception!!!
	};

	VerifyFunctor() {
	}

	template<typename R, typename ... arglist>
	MethodVerificationProgress& operator()(const ProcedureStubbingRoot<R, arglist...>& verificationProgress) {
		ProcedureStubbingRoot<R, arglist...>& verificationProgressWithoutConst =
				const_cast<ProcedureStubbingRoot<R, arglist...>&>(verificationProgress);
		verificationProgressWithoutConst.startVerification();
		return verificationProgressWithoutConst;
	}

	template<typename R, typename ... arglist>
	MethodVerificationProgress& operator()(const FunctionStubbingRoot<R, arglist...>& verificationProgress) {
		FunctionStubbingRoot<R, arglist...>& verificationProgressWithoutConst =
				const_cast<FunctionStubbingRoot<R, arglist...>&>(verificationProgress);
		verificationProgressWithoutConst.startVerification();
		return verificationProgressWithoutConst;
	}

	VerificationProgress operator()(const Sequence& sequence) {
		Sequence& sequenceWithoutConst = const_cast<Sequence&>(sequence);
		sequenceWithoutConst.startVerification();
		VerificationProgress v(sequence);
		return v;
	}

}static Verify;

class WhenFunctor {
public:
	WhenFunctor() {
	}

	template<typename R, typename ... arglist>
	FirstProcedureStubbingProgress<R, arglist...>& operator()(const ProcedureStubbingRoot<R, arglist...>& stubbingProgress) {
		return (FirstProcedureStubbingProgress<R, arglist...>&) stubbingProgress;
	}

	template<typename R, typename ... arglist>
	FirstFunctionStubbingProgress<R, arglist...>& operator()(const FunctionStubbingRoot<R, arglist...>& stubbingProgress) {
		return (FirstFunctionStubbingProgress<R, arglist...>&) stubbingProgress;
	}

}static When;

class StubFunctor {
private:
	void operator()() {
	}
public:
	StubFunctor() {
	}

	template<typename H>
	void operator()(const H& head) {
		H& headWithoutConst = const_cast<H&>(head);
		headWithoutConst.startStubbing();
	}

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
		H& headWithoutConst = const_cast<H&>(head);
		headWithoutConst.startStubbing();
		this->operator()(tail...);
	}

}static Stub;

}

#endif // ClousesImpl_h__
