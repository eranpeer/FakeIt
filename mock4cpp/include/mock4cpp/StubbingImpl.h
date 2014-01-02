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
struct FunctionStubbingProgress: public virtual FirstFunctionStubbingProgress<R, arglist...>, //
		public virtual NextFunctionStubbingProgress<R, arglist...> {

	FunctionStubbingProgress() = default;
	virtual ~FunctionStubbingProgress() override = default;

	NextFunctionStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		InvocationMock().appendDo(method);
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		InvocationMock().clear();
		InvocationMock().appendDo(method);
		return *this;
	}

protected:
	virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() = 0;
private:
	FunctionStubbingProgress & operator=(const FunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct ProcedureStubbingProgress: public virtual FirstProcedureStubbingProgress<R, arglist...>, //
		public virtual NextProcedureStubbingProgress<R, arglist...> {

	ProcedureStubbingProgress() = default;
	~ProcedureStubbingProgress() override = default;

	NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		InvocationMock().appendDo(method);
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		InvocationMock().clear();
		InvocationMock().appendDo(method);
		return *this;
	}

protected:
	virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() = 0;
private:
	ProcedureStubbingProgress & operator=(const ProcedureStubbingProgress & other) = delete;
};

using namespace mock4cpp;

template<typename R, typename ... arglist>
class MethodStubbingBase: public virtual MethodStubbingInternal, public virtual MethodVerificationProgress {

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

protected:
	std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext;
	std::shared_ptr<MethodInvocationMockBase<R, arglist...>> invocationMock;
	ProgressType progressType;
	int expectedInvocationCount;

	MethodStubbingBase(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			stubbingContext(stubbingContext), invocationMock(nullptr), progressType(ProgressType::NONE), expectedInvocationCount(-1) {
	}

	int CountInvocations(MethodInvocationMockBase<R, arglist...> &invocationMock) {
		int times = stubbingContext->getMethodMock().getActualInvocations(invocationMock).size();
		return times;
	}

	void initInvocationMockIfNeeded() {
		if (!invocationMock) {
			auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
			invocationMock = std::shared_ptr<MethodInvocationMockBase<R, arglist...>> { new DefaultInvocationMock<R, arglist...>(
					initialMethodBehavior) };
		}
	}

	virtual ~MethodStubbingBase() THROWS {
		if (progressType == ProgressType::NONE) {
			return;
		}

		initInvocationMockIfNeeded();

		if (progressType == ProgressType::STUBBING) {
			stubbingContext->getMethodMock().stubMethodInvocation(invocationMock);
			return;
		}

		if (progressType == ProgressType::VERIFYING) {
			auto actualInvocations = CountInvocations(*invocationMock);
			invocationMock = nullptr;

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

public:

	virtual void clearProgress() {
		progressType = ProgressType::NONE;
		if (invocationMock) {
			invocationMock = nullptr;
		}
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
class FunctionStubbingRoot: private virtual MethodStubbingBase<R, arglist...>, private virtual FunctionStubbingProgress<R, arglist...> {
private:
	FunctionStubbingRoot & operator=(const FunctionStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

protected:

	virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override {
		return *MethodStubbingBase<R, arglist...>::invocationMock;
	}

public:

	FunctionStubbingRoot(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			MethodStubbingBase<R, arglist...>(stubbingContext), FirstFunctionStubbingProgress<R, arglist...>(), FunctionStubbingProgress<R,
					arglist...>() {
	}

	FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

	~FunctionStubbingRoot() THROWS {}

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstFunctionStubbingProgress<R, arglist...>::operator =(method);
	}

	FunctionStubbingRoot<R, arglist...>& Using(const arglist&... args) {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		MethodStubbingBase<R, arglist...>::invocationMock = std::shared_ptr<MethodInvocationMockBase<R, arglist...>> {
				new ExpectedInvocationMock<R, arglist...>(args..., initialMethodBehavior) };
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		MethodStubbingBase<R, arglist...>::invocationMock = std::shared_ptr<MethodInvocationMockBase<R, arglist...>> {
				new MatchingInvocationMock<R, arglist...>(matcher, initialMethodBehavior) };
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<R, arglist...>::startStubbing();
		MethodStubbingBase<R, arglist...>::initInvocationMockIfNeeded();
		return FunctionStubbingProgress<R, arglist...>::Do(method);
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
	virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override {
		return *MethodStubbingBase<R, arglist...>::invocationMock;
	}
public:
	ProcedureStubbingRoot(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			MethodStubbingBase<R, arglist...>(stubbingContext), FirstProcedureStubbingProgress<R, arglist...>(), ProcedureStubbingProgress<
					R, arglist...>() {
	}

	~ProcedureStubbingRoot() THROWS {}

	ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstProcedureStubbingProgress<R, arglist...>::operator =(method);
	}

	ProcedureStubbingRoot<R, arglist...>& Using(const arglist&... args) {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		MethodStubbingBase<R, arglist...>::invocationMock = std::shared_ptr<MethodInvocationMockBase<R, arglist...>> {
				new ExpectedInvocationMock<R, arglist...>(args..., initialMethodBehavior) };
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		MethodStubbingBase<R, arglist...>::invocationMock = std::shared_ptr<MethodInvocationMockBase<R, arglist...>> {
				new MatchingInvocationMock<R, arglist...>(matcher, initialMethodBehavior) };
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<R, arglist...>::startStubbing();
		MethodStubbingBase<R, arglist...>::initInvocationMockIfNeeded();
		return ProcedureStubbingProgress<R, arglist...>::Do(method);
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
