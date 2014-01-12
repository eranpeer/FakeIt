#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>
#include <memory>

#include "mock4cpp/MethodMock.h"
#include "mock4cpp/Stubbing.h"
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
		protected virtual MethodVerificationProgress {

private:

	std::shared_ptr<RecordedMethodBody<R, arglist...>> buildInitialMethodBody() {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody { new RecordedMethodBody<R, arglist...>() };
		recordedMethodBody->appendDo(initialMethodBehavior);
		return recordedMethodBody;
	}

	std::shared_ptr<MethodInvocationMockBase<R, arglist...>> buildInitialMethodInvocationMock(
			std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher) {
		return std::shared_ptr<MethodInvocationMockBase<R, arglist...>> { new MethodInvocationMockBase<R, arglist...>(invocationMatcher,
				recordedMethodBody) };
	}

	void initInvocationMockIfNeeded() {
		if (!invocationMock) {
			MethodStubbingBase<R, arglist...>::invocationMock = MethodStubbingBase<R, arglist...>::buildInitialMethodInvocationMock(
					invocationMatcher);
		}
	}

protected:
	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

	std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext;
	std::shared_ptr<MethodInvocationMockBase<R, arglist...>> invocationMock;
	std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher;
	std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody;

	ProgressType progressType;
	int expectedInvocationCount;

	MethodStubbingBase(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			stubbingContext(stubbingContext), invocationMock(nullptr), invocationMatcher { new DefaultInvocationMatcher<arglist...>() }, progressType(
					ProgressType::NONE), expectedInvocationCount(-1) {
				recordedMethodBody =  buildInitialMethodBody();
	}

	int CountInvocations(InvocationMatcher<arglist...>& invocationMatcher) {
		int times = stubbingContext->getMethodMock().getActualInvocations(invocationMatcher).size();
		return times;
	}


	virtual ~MethodStubbingBase() THROWS {
		if (progressType == ProgressType::NONE) {
			return;
		}

		if (progressType == ProgressType::STUBBING) {
			initInvocationMockIfNeeded();
			stubbingContext->getMethodMock().stubMethodInvocation(invocationMock);
			return;
		}

		if (progressType == ProgressType::VERIFYING) {
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

	virtual void clearProgress() {
		progressType = ProgressType::NONE;
//		if (invocationMock) {
//			invocationMock = nullptr;
//		}
	}

	virtual void startStubbing() {
		progressType = ProgressType::STUBBING;
	}

	virtual void startVerification() override {
		progressType = ProgressType::VERIFYING;
	}

	virtual void VerifyInvocations(const int times) override {
		startVerification();
		expectedInvocationCount = times;
	}

};

template<typename R, typename ... arglist>
class FunctionStubbingRoot: //
private virtual MethodStubbingBase<R, arglist...>, //
		private virtual FunctionStubbingProgress<R, arglist...> {
private:
	FunctionStubbingRoot & operator=(const FunctionStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

protected:

	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() override {
		return *MethodStubbingBase<R, arglist...>::recordedMethodBody;
	}

	virtual void startStubbing() override {
		MethodStubbingBase<R, arglist...>::startStubbing();
	}

	virtual void VerifyInvocations(const int times) override {
		MethodStubbingBase<R, arglist...>::VerifyInvocations(times);
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void startVerification() override {
		MethodStubbingBase<R, arglist...>::startVerification();
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void clearProgress() override {
		MethodStubbingBase<R, arglist...>::clearProgress();
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
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) };
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) };
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) };
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) };
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
private virtual MethodStubbingBase<R, arglist...>,
		private virtual ProcedureStubbingProgress<R, arglist...> {
private:
	ProcedureStubbingRoot & operator=(const ProcedureStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() override {
		return *MethodStubbingBase<R, arglist...>::recordedMethodBody;
	}

	virtual void startStubbing() override {
		MethodStubbingBase<R, arglist...>::startStubbing();
	}

	virtual void VerifyInvocations(const int times) override {
		MethodStubbingBase<R, arglist...>::VerifyInvocations(times);
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void startVerification() override {
		MethodStubbingBase<R, arglist...>::startVerification();
	}

	// put method here to silent the MSC++ warning C4250: inherits via dominance
	virtual void clearProgress() override {
		MethodStubbingBase<R, arglist...>::clearProgress();
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
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) };
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) };
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) };
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) };
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
