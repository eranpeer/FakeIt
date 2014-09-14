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
template<typename R, typename ... arglist>
class MethodMockingContext: //
		public Sequence,                // For use in Verify(sequence1,...)... phrases.
		public ActualInvocationsSource, // For use in Using(source1,souece2,...) and VerifyNoOtherInvocations(source1,souece2...) phrases.
		public virtual StubbingContext<R, arglist...>, // For use in Fake, Spy & When phrases
		public virtual SpyingContext<R, arglist...>, // For use in Fake, Spy & When phrases
		private Invocation::Matcher {

public:

	struct Context: public Destructable {


		virtual ~Context() = default;

		/**
		 * Return the original method. not the mock.
		 */
		virtual std::function<R(arglist&...)> getOriginalMethod() = 0;

		virtual std::string getMethodName() = 0;

		virtual void addMethodInvocationHandler(typename ActualInvocation<arglist...>::Matcher* matcher,
							MethodInvocationHandler<R, arglist...>* invocationHandler) = 0;

		virtual void scanActualInvocations(const std::function<void(ActualInvocation<arglist...>&)>& scanner) = 0;

		virtual void setMethodDetails(std::string mockName,std::string methodName) = 0;

		virtual bool isOfMethod(Method& method) =  0;

		virtual ActualInvocationsSource& getInvolvedMock() = 0;
	};

protected:

	MethodMockingContext(Context* stubbingContext) :
	_stubbingContext(stubbingContext), _invocationMatcher {new DefaultInvocationMatcher<arglist...>()}, _expectedInvocationCount(-1), _commited(false) {
		_recordedActionSequence = new ActionSequence<R, arglist...>();
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
		std::string s = getStubbingContext().getMethodName();
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
		into.insert(&getStubbingContext().getInvolvedMock());
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
		auto scanner = [&](ActualInvocation<arglist...>& a){
			if (_invocationMatcher->matches(a)){
				into.insert(&a);
			}
		};
		getStubbingContext().scanActualInvocations(scanner);
	}

	Context& getStubbingContext() const {
		Destructable& destructable = *_stubbingContext;
		Context& rv = dynamic_cast<Context&>(destructable);
		return rv;
	}

	/**
	 * Used only by Verify phrase.
	 */
	virtual bool matches(Invocation& invocation) override {
		Method& actualMethod = invocation.getMethod();
		if (!getStubbingContext().isOfMethod(actualMethod)) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return _invocationMatcher->matches(actualInvocation);
	}

	virtual void commit() override {
		getStubbingContext().addMethodInvocationHandler(_invocationMatcher, _recordedActionSequence);
		_commited = true;
	}

	void setMethodDetails(std::string mockName,std::string methodName) {
		getStubbingContext().setMethodDetails(mockName,methodName);
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

template<typename R, typename ... arglist>
class FunctionMockingContext: //
public virtual MethodMockingContext<R, arglist...> //
{
	FunctionMockingContext & operator=(const FunctionMockingContext&) = delete;

public:

	FunctionMockingContext(typename MethodMockingContext<R, arglist...>::Context* stubbingContext) :
			MethodMockingContext<R, arglist...>(stubbingContext) {
	}

	FunctionMockingContext(FunctionMockingContext<R, arglist...>&other) :MethodMockingContext<R, arglist...>(other){}
	FunctionMockingContext(FunctionMockingContext<R, arglist...>&&other):MethodMockingContext<R, arglist...>(other){}

	virtual ~FunctionMockingContext() THROWS {
	}

	void operator=(std::function<R(arglist...)> method) {
		MethodMockingContext<R, arglist...>::setMethodBodyByAssignment(method);
	}

	FunctionMockingContext<R, arglist...>& setMethodDetails(std::string mockName, std::string methodName) {
		MethodMockingContext<R, arglist...>::setMethodDetails(mockName, methodName);
		return *this;
	}

	FunctionMockingContext<R, arglist...>& Using(const arglist&... args) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	FunctionMockingContext<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	FunctionMockingContext<R, arglist...>& operator()(const arglist&... args) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	FunctionMockingContext<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [r](arglist&...) -> R {return r;};
		MethodMockingContext<R, arglist...>::appendAction(new RepeatForever<R, arglist...>(method));
		MethodMockingContext<R, arglist...>::commit();
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value, void>::type operator=(const R& r) {
		auto method = [&r](arglist&...) -> R {return r;};
		MethodMockingContext<R, arglist...>::appendAction(new RepeatForever<R, arglist...>(method));
		MethodMockingContext<R, arglist...>::commit();
	}
};

template<typename R, typename ... arglist>
class ProcedureMockingContext: //
public virtual MethodMockingContext<R, arglist...> {

	ProcedureMockingContext & operator=(const ProcedureMockingContext&) = delete;

public:
	ProcedureMockingContext(typename MethodMockingContext<R, arglist...>::Context* stubbingContext) :
			MethodMockingContext<R, arglist...>(stubbingContext) {
	}

	virtual ~ProcedureMockingContext() THROWS {
	}

	ProcedureMockingContext(ProcedureMockingContext<R, arglist...>& other):MethodMockingContext<R, arglist...>(other){}
	ProcedureMockingContext(ProcedureMockingContext<R, arglist...>&& other):MethodMockingContext<R, arglist...>(other){}

	void operator=(std::function<R(arglist...)> method) {
		MethodMockingContext<R, arglist...>::setMethodBodyByAssignment(method);
	}

	ProcedureMockingContext<R, arglist...>& setMethodDetails(std::string mockName, std::string methodName) {
		MethodMockingContext<R, arglist...>::setMethodDetails(mockName, methodName);
		return *this;
	}

	ProcedureMockingContext<R, arglist...>& Using(const arglist&... args) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	ProcedureMockingContext<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}

	ProcedureMockingContext<R, arglist...>& operator()(const arglist&... args) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(args...);
		return *this;
	}

	ProcedureMockingContext<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodMockingContext<R, arglist...>::setMatchingCriteria(matcher);
		return *this;
	}
};

}
#endif
