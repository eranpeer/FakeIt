#ifndef StubbingImpl_h__
#define StubbingImpl_h__

#include <functional>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>
#include <iostream>

#include "fakeit/MethodMock.h"
#include "fakeit/Stubbing.h"
#include "fakeit/Sequence.hpp"
#include "fakeit/ActualInvocation.h"
#include "fakeit/InvocationMatcher.h"
#include "fakeit/ErrorFormatter.h"
#include "fakeit/DefaultErrorFormatter.h"
#include "mockutils/ExtractMemberType.h"

namespace fakeit {

enum class ProgressType {
	NONE, STUBBING, VERIFYING
};

template<typename C, typename R, typename ... arglist>
struct StubbingContext {
	virtual ~StubbingContext() {
	}
	virtual MethodMock<C, R, arglist...>& getMethodMock() = 0;
};


struct Mock4cppRoot {
	Mock4cppRoot(ErrorFormatter& errorFormatter) :
			errorFormatter(errorFormatter) {
	}
	void setErrorFormatter(ErrorFormatter& ef) {
		errorFormatter = ef;
	}

	ErrorFormatter& getErrorFromatter() {
		return errorFormatter;
	}

private:
	ErrorFormatter& errorFormatter;
}static Mock4cpp(defaultErrorFormatter);

struct Xaction {
	virtual void apply() = 0;
};

template<typename C, typename R, typename ... arglist>
class MethodStubbingBase: public Xaction,//
protected virtual MethodStubbingInternal,
		public virtual Sequence,
		public virtual ActualInvocationsSource,
		public virtual AnyInvocation::Matcher {
private:

	std::shared_ptr<RecordedMethodBody<R, arglist...>> buildInitialMethodBody() {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody { new RecordedMethodBody<R, arglist...>() };
		recordedMethodBody->appendDo(initialMethodBehavior);
		return recordedMethodBody;
	}

	void setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher) {
		MethodStubbingBase<C, R, arglist...>::invocationMatcher = invocationMatcher;
	}

protected:
	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

	std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext;
	std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher;
	std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody;

	int expectedInvocationCount;

	MethodStubbingBase(std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext) :
			stubbingContext(stubbingContext), invocationMatcher { new DefaultInvocationMatcher<arglist...>() }, expectedInvocationCount(-1) {
		recordedMethodBody = buildInitialMethodBody();
	}

	virtual ~MethodStubbingBase() THROWS {
	}

	virtual void apply() override {
		stubbingContext->getMethodMock().stubMethodInvocation(invocationMatcher, recordedMethodBody);
	}

public:
	virtual bool matches(AnyInvocation& invocation) override {
		if (&invocation.getMethod() != &stubbingContext->getMethodMock()) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return invocationMatcher->matches(actualInvocation);
	}

	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>>actualInvocations = stubbingContext->getMethodMock().getActualInvocations(*invocationMatcher);
		for (auto i : actualInvocations) {
			AnyInvocation* ai = i.get();
			into.insert(ai);
		}
	}

	void getExpectedSequence(std::vector<AnyInvocation::Matcher*>& into) const override {
		const AnyInvocation::Matcher* b = this;
		AnyInvocation::Matcher* c = const_cast<AnyInvocation::Matcher*>(b);
		into.push_back(c);
	}

	unsigned int size() const override {
		return 1;
	}

	void Using(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
	}

	void Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
	}

	void operator()(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
	}

	void operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
	}

	void FirstAction(std::function<R(arglist...)> method) {
		recordedMethodBody->clear();
		recordedMethodBody->appendDo(method);
	}

	void AnotherAction(std::function<R(arglist...)> method) {
		recordedMethodBody->appendDo(method);
	}

};

template<typename C, typename R, typename ... arglist>
class FunctionStubbingRoot: //
public virtual MethodStubbingBase<C, R, arglist...>, //
		private virtual FunctionStubbingProgress<R, arglist...> {
private:
	FunctionStubbingRoot & operator=(const FunctionStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;
protected:

public:

	FunctionStubbingRoot(std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext) :
			MethodStubbingBase<C, R, arglist...>(stubbingContext), FirstFunctionStubbingProgress<R, arglist...>(), FunctionStubbingProgress<
					R, arglist...>() {
	}

	FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

	virtual ~FunctionStubbingRoot() THROWS {
	}

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstFunctionStubbingProgress<R, arglist...>::operator =(method);
		MethodStubbingBase<C, R, arglist...>::apply();
	}

	FunctionStubbingRoot<C, R, arglist...>& Using(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::Using(args...);
		return *this;
	}

	FunctionStubbingRoot<C, R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::Matching(matcher);
		return *this;
	}

	FunctionStubbingRoot<C, R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::operator()(args...);
		return *this;
	}

	FunctionStubbingRoot<C, R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::operator()(matcher);
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<C, R, arglist...>::FirstAction(method);
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		MethodStubbingBase<C, R, arglist...>::AnotherAction(method);
		return *this;
	}

};

template<typename C, typename R, typename ... arglist>
class ProcedureStubbingRoot: //
public virtual MethodStubbingBase<C, R, arglist...>,
		private virtual ProcedureStubbingProgress<R, arglist...> {
private:
	ProcedureStubbingRoot & operator=(const ProcedureStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

protected:

public:
	ProcedureStubbingRoot(std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext) :
			MethodStubbingBase<C, R, arglist...>(stubbingContext), FirstProcedureStubbingProgress<R, arglist...>(), ProcedureStubbingProgress<
					R, arglist...>() {
	}

	virtual ~ProcedureStubbingRoot() THROWS {
	}

	ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstProcedureStubbingProgress<R, arglist...>::operator=(method);
		MethodStubbingBase<C, R, arglist...>::apply();
	}

	ProcedureStubbingRoot<C, R, arglist...>& Using(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::Using(args...);
		return *this;
	}

	ProcedureStubbingRoot<C, R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::Matching(matcher);
		return *this;
	}

	ProcedureStubbingRoot<C, R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::operator()(args...);
		return *this;
	}

	ProcedureStubbingRoot<C, R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::operator()(matcher);
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<C, R, arglist...>::FirstAction(method);
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		MethodStubbingBase<C, R, arglist...>::AnotherAction(method);
		return *this;
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

}
#endif // StubbingImpl_h__
