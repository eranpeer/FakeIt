/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef MethodMockingContext_h__
#define MethodMockingContext_h__

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
#include "fakeit/DomainObjects.hpp"

namespace fakeit {

enum class ProgressType {
	NONE, STUBBING, VERIFYING
};


struct Xaction {
	virtual void commit() = 0;
};

// For use in Spy(...) phrases
template<typename R, typename ... arglist>
struct SpyingContext : public Xaction {
	virtual void appendAction(Action<R, arglist...>* action) = 0;
	virtual typename std::function<R(arglist&...)> getOriginalMethod() = 0;
};

// For use in Fake & When phrases
template<typename R, typename ... arglist>
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
class MethodMockingContext: //
		public Sequence,                // For use in Verify(sequence1,...)... phrases.
		public ActualInvocationsSource, // For use in Using(source1,souece2,...) and VerifyNoOtherInvocations(source1,souece2...) phrases.
		public virtual StubbingContext<R, arglist...>, // For use in Fake, Spy & When phrases
		public virtual SpyingContext<R, arglist...>, // For use in Fake, Spy & When phrases
		private Invocation::Matcher {

public:

	struct MethodMockingContextContext: public ActualInvocationsSource, public Destructable {


		virtual ~MethodMockingContextContext() = default;

		virtual RecordedMethodBody<C, R, arglist...>& getRecordedMethodBody() = 0;

		virtual MockObject<C>& getMock() = 0;

		virtual C& getMockInstance() = 0;

		/**
		 * Return the original method. not the mock.
		 */
		virtual std::function<R(arglist&...)> getOriginalMethod() = 0;

	};

protected:

	MethodMockingContext(MethodMockingContextContext* stubbingContext) :
	_stubbingContext(stubbingContext), _invocationMatcher {new DefaultInvocationMatcher<arglist...>()}, _expectedInvocationCount(-1), _commited(false) {
		_recordedActionSequence = buildInitialRecordedSequence();
	}

	//Move ctor for use by derived classes.
	MethodMockingContext(MethodMockingContext& other) :
		_stubbingContext(other._stubbingContext),
		_invocationMatcher {other._invocationMatcher},
		_recordedActionSequence(other._recordedActionSequence),
		_expectedInvocationCount(other._expectedInvocationCount),
		_commited(other._commited)
	{
		other._recordedActionSequence = nullptr;
		other._invocationMatcher = nullptr;
		other._commited = true; // avoid delete by other
	}


	virtual ~MethodMockingContext() {
		// committed objects will be deleted by the context.
		if (!_commited) {
			// no commit. delete the created objects.
			delete _invocationMatcher;
			delete _recordedActionSequence;
		}
	}

	std::string format() const {
		std::string s = getStubbingContext().getRecordedMethodBody().getMethod().name();
		s += _invocationMatcher->format();
		return s;
	}

	unsigned int size() const override {
		return 1;
	}

	/**
	 * Used only by Verify phrase.
	 */
	void getInvolvedMocks(std::set<const ActualInvocationsSource*>& into) const override {
		into.insert(&getStubbingContext());
	}

	void getExpectedSequence(std::vector<Invocation::Matcher*>& into) const override {
		const Invocation::Matcher* b = this;
		Invocation::Matcher* c = const_cast<Invocation::Matcher*>(b);
		into.push_back(c);
	}

	/**
	 * Used only by Verify phrase.
	 */
	void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
		getStubbingContext().getRecordedMethodBody().scanActualInvocations(
				[&](ActualInvocation<arglist...>& a){
			if (_invocationMatcher->matches(a)){
				into.insert(&a);
			}
		});
	}

	MethodMockingContextContext& getStubbingContext() const {
		Destructable& destructable = *_stubbingContext;
		MethodMockingContextContext& rv = dynamic_cast<MethodMockingContextContext&>(destructable);
		return rv;
	}

	/**
	 * Used only by Verify phrase.
	 */
	virtual bool matches(Invocation& invocation) override {
		RecordedMethodBody<C, R, arglist...>& recordedMethodBody = getStubbingContext().getRecordedMethodBody();
		Method& actualMethod = invocation.getMethod();
		if (!recordedMethodBody.isOfMethod(actualMethod)) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return _invocationMatcher->matches(actualInvocation);
	}

	virtual void commit() override {
		getStubbingContext().getRecordedMethodBody().addMethodInvocationHandler(_invocationMatcher, _recordedActionSequence);
		_commited = true;
	}

	void setMethodDetails(std::string mockName,std::string methodName) {
		getStubbingContext().getRecordedMethodBody().setMethodDetails(mockName,methodName);
	}

	void setMatchingCriteria(const arglist&... args) {
		typename ActualInvocation<arglist...>::Matcher* matcher {
			new ExpectedArgumentsInvocationMatcher<arglist...>(args...)};
		setInvocationMatcher(matcher);
	}

	void setMatchingCriteria(std::function<bool(arglist...)> predicate) {
		typename ActualInvocation<arglist...>::Matcher* matcher {
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

	typename std::function<R(arglist&...)> getOriginalMethod() {
		return getStubbingContext().getOriginalMethod();
	}

	C& get() {
		return getStubbingContext().getMock().get();
	}

	ActionSequence<R, arglist...>* buildInitialRecordedSequence() {
		return new ActionSequence<R, arglist...>(getStubbingContext().getRecordedMethodBody().getMethod());
	}

	void setInvocationMatcher(typename ActualInvocation<arglist...>::Matcher* matcher) {
		delete _invocationMatcher;
		_invocationMatcher = matcher;
	}

	std::shared_ptr<Destructable> _stubbingContext;
	typename ActualInvocation<arglist...>::Matcher* _invocationMatcher;
	ActionSequence<R, arglist...>* _recordedActionSequence;
	int _expectedInvocationCount;
	bool _commited;
};

template<typename C, typename R, typename ... arglist>
class FunctionSequenceBuilder: //
public virtual MethodMockingContext<C, R, arglist...> //
{
private:
	FunctionSequenceBuilder & operator=(const FunctionSequenceBuilder&) = delete;

protected:

public:

	FunctionSequenceBuilder(typename MethodMockingContext<C, R, arglist...>::MethodMockingContextContext* stubbingContext) :
			MethodMockingContext<C, R, arglist...>(stubbingContext) {
	}

	FunctionSequenceBuilder(FunctionSequenceBuilder<C, R, arglist...>&other) :MethodMockingContext<C, R, arglist...>(other){}
	FunctionSequenceBuilder(FunctionSequenceBuilder<C, R, arglist...>&&other):MethodMockingContext<C, R, arglist...>(other){}

	virtual ~FunctionSequenceBuilder() THROWS {
	}

	void operator=(std::function<R(arglist...)> method) {
		MethodMockingContext<C, R, arglist...>::setMethodBodyByAssignment(method);
	}

	FunctionSequenceBuilder<C, R, arglist...>& setMethodDetails(std::string mockName, std::string methodName) {
		MethodMockingContext<C, R, arglist...>::setMethodDetails(mockName, methodName);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& Using(const arglist&... args) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& operator()(const arglist&... args) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	FunctionSequenceBuilder<C, R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [r](arglist&...) -> R {return r;};
		MethodMockingContext<C, R, arglist...>::appendAction(new RepeatForever<R, arglist...>(method));
		MethodMockingContext<C, R, arglist...>::commit();
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [&r](arglist&...) -> R {return r;};
		MethodMockingContext<C, R, arglist...>::appendAction(new RepeatForever<R, arglist...>(method));
		MethodMockingContext<C, R, arglist...>::commit();
	}
};

template<typename C, typename R, typename ... arglist>
class ProcedureSequenceBuilder: //
public virtual MethodMockingContext<C, R, arglist...> {
private:
	ProcedureSequenceBuilder & operator=(const ProcedureSequenceBuilder&) = delete;

protected:

public:
	ProcedureSequenceBuilder(typename MethodMockingContext<C, R, arglist...>::MethodMockingContextContext* stubbingContext) :
			MethodMockingContext<C, R, arglist...>(stubbingContext) {
	}

	virtual ~ProcedureSequenceBuilder() THROWS {
	}

	ProcedureSequenceBuilder(ProcedureSequenceBuilder<C, R, arglist...>& other):MethodMockingContext<C, R, arglist...>(other){}
	ProcedureSequenceBuilder(ProcedureSequenceBuilder<C, R, arglist...>&& other):MethodMockingContext<C, R, arglist...>(other){}

	void operator=(std::function<R(arglist...)> method) {
		MethodMockingContext<C, R, arglist...>::setMethodBodyByAssignment(method);
	}

	ProcedureSequenceBuilder<C, R, arglist...>& setMethodDetails(std::string mockName, std::string methodName) {
		MethodMockingContext<C, R, arglist...>::setMethodDetails(mockName, methodName);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& Using(const arglist&... args) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& operator()(const arglist&... args) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	ProcedureSequenceBuilder<C, R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<C, R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}
};

}
#endif
