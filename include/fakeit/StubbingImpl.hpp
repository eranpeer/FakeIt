#ifndef StubbingImpl_h__
#define StubbingImpl_h__

#include <functional>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>
#include <iostream>

#include "mockutils/ExtractMemberType.hpp"

#include "fakeit/MethodMock.hpp"
#include "fakeit/Stubbing.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/ErrorFormatter.hpp"

namespace fakeit {

enum class ProgressType {
	NONE, STUBBING, VERIFYING
};

template<typename C, typename R, typename ... arglist>
struct StubbingContext : public ActualInvocationsSource {
	virtual ~StubbingContext() {
	}
	virtual MethodMock<C, R, arglist...>& getMethodMock() = 0;
};

struct RecordedMethodInvocation {
	virtual void apply() = 0;
};

template<typename C, typename R, typename ... arglist>
class MethodStubbingBase: public RecordedMethodInvocation, //
		public virtual Sequence,
		public virtual ActualInvocationsSource,
		public virtual Invocation::Matcher {
private:

	std::shared_ptr<RecordedMethodBody<R, arglist...>> buildInitialMethodBody() {
		std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody { new RecordedMethodBody<R, arglist...>() };
		return recordedMethodBody;
	}

	void setInvocationMatcher(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> invocationMatcher) {
		MethodStubbingBase<C, R, arglist...>::invocationMatcher = invocationMatcher;
	}

protected:
	friend class VerifyFunctor;
	friend class FakeFunctor;
	friend class WhenFunctor;

	std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext;
	std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> invocationMatcher;
	std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody;

	int expectedInvocationCount;

	MethodStubbingBase(std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext) :
			stubbingContext(stubbingContext), invocationMatcher { new DefaultInvocationMatcher<arglist...>() }, expectedInvocationCount(-1) {
		recordedMethodBody = buildInitialMethodBody();
	}

	virtual ~MethodStubbingBase() {
	}

	virtual void apply() override {
		stubbingContext->getMethodMock().stubMethodInvocation(invocationMatcher, recordedMethodBody);
	}

public:
	virtual bool matches(Invocation& invocation) override {
		if (&invocation.getMethod() != &stubbingContext->getMethodMock().getMethod()) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return invocationMatcher->matches(actualInvocation);
	}

	void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>>actualInvocations = stubbingContext->getMethodMock().getActualInvocations(*invocationMatcher);
		for (auto i : actualInvocations) {
			Invocation* ai = i.get();
			into.insert(ai);
		}
	}

	void getInvolvedMocks(std::set<ActualInvocationsSource*>& into) const override {
		into.insert(stubbingContext.get());
	}

	void getExpectedSequence(std::vector<Invocation::Matcher*>& into) const override {
		const Invocation::Matcher* b = this;
		Invocation::Matcher* c = const_cast<Invocation::Matcher*>(b);
		into.push_back(c);
	}

	unsigned int size() const override {
		return 1;
	}

	void Using(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> {
					new ExpectedArgumentsInvocationMatcher<arglist...>(args...)});
	}

	void Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> {
					new UserDefinedInvocationMatcher<arglist...>(matcher)});
	}

	void operator()(const arglist&... args) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> {
					new ExpectedArgumentsInvocationMatcher<arglist...>(args...)});
	}

	void operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<C, R, arglist...>::setInvocationMatcher(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> {
					new UserDefinedInvocationMatcher<arglist...>(matcher)});
	}

	void AppendAction(std::shared_ptr<BehaviorMock<R, arglist...>> method) {
		recordedMethodBody->AppendDo(method);
	}

	void operator=(std::function<R(arglist...)> method) {
		std::shared_ptr<BehaviorMock<R, arglist...>> ptr {new DoForeverMock<R, arglist...>(method)};
		MethodStubbingBase<C, R, arglist...>::AppendAction(ptr);
		MethodStubbingBase<C, R, arglist...>::apply();
	}

};

template<typename C, typename R, typename ... arglist>
class FunctionStubbingRoot: //
public virtual MethodStubbingBase<C, R, arglist...> //
{
private:
	FunctionStubbingRoot & operator=(const FunctionStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class FakeFunctor;
	friend class WhenFunctor;
protected:

public:

	FunctionStubbingRoot(std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext) :
			MethodStubbingBase<C, R, arglist...>(stubbingContext) {
	}

	FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

	virtual ~FunctionStubbingRoot() THROWS {
	}

	void operator=(std::function<R(arglist...)> method) {
		MethodStubbingBase<C, R, arglist...>::operator=(method);
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

	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [r](arglist&...) -> R {return r;};
		std::shared_ptr<BehaviorMock<R, arglist...>> ptr { new DoForeverMock<R, arglist...>(method) };
		MethodStubbingBase<C, R, arglist...>::AppendAction(ptr);
		MethodStubbingBase<C, R, arglist...>::apply();
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [&r](arglist&...) -> R {return r;};
		std::shared_ptr<BehaviorMock<R, arglist...>> ptr { new DoForeverMock<R, arglist...>(method) };
		MethodStubbingBase<C, R, arglist...>::AppendAction(ptr);
		MethodStubbingBase<C, R, arglist...>::apply();
	}

};

template<typename C, typename R, typename ... arglist>
class ProcedureStubbingRoot: //
public virtual MethodStubbingBase<C, R, arglist...> {
private:
	ProcedureStubbingRoot & operator=(const ProcedureStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class FakeFunctor;
	friend class WhenFunctor;

protected:

public:
	ProcedureStubbingRoot(std::shared_ptr<StubbingContext<C, R, arglist...>> stubbingContext) :
			MethodStubbingBase<C, R, arglist...>(stubbingContext) {
	}

	virtual ~ProcedureStubbingRoot() THROWS {
	}

	ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

	void operator=(std::function<R(arglist...)> method) {
		MethodStubbingBase<C, R, arglist...>::operator=(method);
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
