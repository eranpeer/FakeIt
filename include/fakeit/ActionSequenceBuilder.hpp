/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef ActionSequenceBuilder_h__
#define ActionSequenceBuilder_h__

#include <functional>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>
#include <iosfwd>

#include "fakeit/RecordedMethodBody.hpp"
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/EventHandler.hpp"
#include "fakeit/ActionSequence.hpp"

namespace fakeit {

enum class ProgressType {
	NONE, STUBBING, VERIFYING
};

template<typename C, typename R, typename ... arglist>
struct ActionSequenceBuilderContext: public ActualInvocationsSource {

	typedef R (C::*MethodType)(arglist...);

	virtual ~ActionSequenceBuilderContext() = default;

	virtual RecordedMethodBody<C, R, arglist...>& getRecordedMethodBody() = 0;

	/**
	 * Return the original method. not the mock.
	 */
	virtual MethodType getOriginalMethod() = 0;

	virtual MockObject<C>& getMock() = 0;

};

struct Xaction {
	virtual void commit() = 0;
};

// For use in Fake, Spy & When phrases
template<typename C, typename R, typename ... arglist>
struct StubbingContext : public Xaction {

	virtual void appendAction(Action<R, arglist...>* action) = 0;

};

/**
 * Build recorded sequence and the matching criteria.
 * For example, for the following line:
 * When(Method(mock,foo)).Return(1).Return(2_Times(2)).Throw(e1);
 * The matching criteria is: Any invocation of mock.foo
 * The recorded sequence is: {Return(1), Return(2), Return(2), Throw(e1)}
 */
template<typename C, typename R, typename ... arglist>
class ActionSequenceBuilder: //
		public Sequence,                // For use in Verify(sequence1,...)... phrases.
		public ActualInvocationsSource, // For use in Using(source1,souece2,...) and VerifyNoOtherInvocations(source1,souece2...) phrases.
		public StubbingContext<C, R, arglist...>, // For use in Fake, Spy & When phrases
		private Invocation::Matcher {

public:

	/**
	 * Used only by Verify phrase.
	 */
	virtual bool matches(Invocation& invocation) override {
		RecordedMethodBody<C, R, arglist...>& recordedMethodBody = _stubbingContext->getRecordedMethodBody();
		Method& actualMethod = invocation.getMethod();
		if (!recordedMethodBody.isOfMethod(actualMethod)) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return _invocationMatcher->matches(actualInvocation);
	}

	/**
	 * Used only by Verify phrase.
	 */
	void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>>actualInvocations =
		_stubbingContext->getRecordedMethodBody().getActualInvocations(*_invocationMatcher);
		for (auto i : actualInvocations) {
			Invocation* ai = i.get();
			into.insert(ai);
		}
	}

	/**
	 * Used only by Verify phrase.
	 */
	void getInvolvedMocks(std::set<const ActualInvocationsSource*>& into) const override {
		into.insert(_stubbingContext.get());
	}

	void getExpectedSequence(std::vector<Invocation::Matcher*>& into) const override {
		const Invocation::Matcher* b = this;
		Invocation::Matcher* c = const_cast<Invocation::Matcher*>(b);
		into.push_back(c);
	}

	std::string format() const {
		std::string s = _stubbingContext->getRecordedMethodBody().getMethod().name();
		s += _invocationMatcher->format();
		return s;
	}

	unsigned int size() const override {
		return 1;
	}

protected:

	ActionSequenceBuilder(std::shared_ptr<ActionSequenceBuilderContext<C, R, arglist...>> stubbingContext) :
	_stubbingContext(stubbingContext), _invocationMatcher {new DefaultInvocationMatcher<arglist...>()}, _expectedInvocationCount(-1) {
		_recordedActionSequence = buildInitialRecordedSequence();
	}

	virtual ~ActionSequenceBuilder() {
	}

	virtual void commit() override {
		_stubbingContext->getRecordedMethodBody().addMethodInvocationHandler(_invocationMatcher, _recordedActionSequence);
	}

	void setMethodDetails(std::string mockName,std::string methodName) {
		_stubbingContext->getRecordedMethodBody().setMethodDetails(mockName,methodName);
	}

	void setMatchingCriteria(const arglist&... args) {
		auto matcher = std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> {
			new ExpectedArgumentsInvocationMatcher<arglist...>(args...)};
		setInvocationMatcher(matcher);
	}

	void setMatchingCriteria(std::function<bool(arglist...)> predicate) {
		auto matcher = std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> {
			new UserDefinedInvocationMatcher<arglist...>(predicate)};
		setInvocationMatcher(matcher);
	}

	/**
	 * Used by Fake, Spy & When functors
	 */
	void appendAction(Action<R, arglist...>* action) override {
		_recordedActionSequence->AppendDo(action);
	}

	void setMethodBodyByAssignment(std::function<R(arglist...)> method) {
		appendAction(new RepeatForever<R, arglist...>(method));
		commit();
	}

private:
	typedef R (C::*func)(arglist...);

	func getOriginalMethod() {
		return _stubbingContext->getOriginalMethod();
	}

	C& get() {
		return _stubbingContext->getMock().get();
	}

	std::shared_ptr<ActionSequence<R, arglist...>> buildInitialRecordedSequence() {
		std::shared_ptr<ActionSequence<R, arglist...>> recordedMethodBody {new ActionSequence<R, arglist...>(_stubbingContext->getRecordedMethodBody().getMethod())};
		return recordedMethodBody;
	}

	void setInvocationMatcher(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> invocationMatcher) {
		_invocationMatcher = invocationMatcher;
	}

	std::shared_ptr<ActionSequenceBuilderContext<C, R, arglist...>> _stubbingContext;
	std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> _invocationMatcher;
	std::shared_ptr<ActionSequence<R, arglist...>> _recordedActionSequence;
	int _expectedInvocationCount;

	friend class VerifyFunctor;
	friend class FakeFunctor;
	friend class SpyFunctor;
	friend class WhenFunctor;
};

template<typename C, typename R, typename ... arglist>
class FunctionSequenceBuilder: //
public virtual ActionSequenceBuilder<C, R, arglist...> //
{
private:
	FunctionSequenceBuilder & operator=(const FunctionSequenceBuilder&) = delete;

	friend class VerifyFunctor;
	friend class FakeFunctor;
	friend class SpyFunctor;
	friend class WhenFunctor;
protected:

public:

	FunctionSequenceBuilder(std::shared_ptr<ActionSequenceBuilderContext<C, R, arglist...>> stubbingContext) :
			ActionSequenceBuilder<C, R, arglist...>(stubbingContext) {
	}

	FunctionSequenceBuilder(const FunctionSequenceBuilder&) = default;

	virtual ~FunctionSequenceBuilder() THROWS {
	}

	void operator=(std::function<R(arglist...)> method) {
		ActionSequenceBuilder<C, R, arglist...>::setMethodBodyByAssignment(method);
	}

	FunctionSequenceBuilder<C, R, arglist...>& setMethodDetails(std::string mockName, std::string methodName) {
		ActionSequenceBuilder<C, R, arglist...>::setMethodDetails(mockName, methodName);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& Using(const arglist&... args) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& operator()(const arglist&... args) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [r](arglist&...) -> R {return r;};
		ActionSequenceBuilder<C, R, arglist...>::appendAction(new RepeatForever<R, arglist...>(method));
		ActionSequenceBuilder<C, R, arglist...>::commit();
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [&r](arglist&...) -> R {return r;};
		ActionSequenceBuilder<C, R, arglist...>::appendAction(new RepeatForever<R, arglist...>(method));
		ActionSequenceBuilder<C, R, arglist...>::commit();
	}
};

template<typename C, typename R, typename ... arglist>
class ProcedureSequenceBuilder: //
public virtual ActionSequenceBuilder<C, R, arglist...> {
private:
	ProcedureSequenceBuilder & operator=(const ProcedureSequenceBuilder&) = delete;

	friend class VerifyFunctor;
	friend class FakeFunctor;
	friend class SpyFunctor;
	friend class WhenFunctor;

protected:

public:
	ProcedureSequenceBuilder(std::shared_ptr<ActionSequenceBuilderContext<C, R, arglist...>> stubbingContext) :
			ActionSequenceBuilder<C, R, arglist...>(stubbingContext) {
	}

	virtual ~ProcedureSequenceBuilder() THROWS {
	}

	ProcedureSequenceBuilder(const ProcedureSequenceBuilder&) = default;

	void operator=(std::function<R(arglist...)> method) {
		ActionSequenceBuilder<C, R, arglist...>::setMethodBodyByAssignment(method);
	}

	ProcedureSequenceBuilder<C, R, arglist...>& setMethodDetails(std::string mockName, std::string methodName) {
		ActionSequenceBuilder<C, R, arglist...>::setMethodDetails(mockName, methodName);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& Using(const arglist&... args) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& operator()(const arglist&... args) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		ActionSequenceBuilder<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}
};

}
#endif
