/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef MethodMock_h__
#define MethodMock_h__

#include <vector>
#include <functional>
#include <atomic>
#include <tuple>

#include "mockutils/TupleDispatcher.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Behavior.hpp"

namespace fakeit {

static std::atomic_int invocationOrdinal;

template<typename R, typename ... arglist>
struct MethodInvocationMock: public ActualInvocation<arglist...>::Matcher, public MethodInvocationHandler<R, arglist...> {

	virtual ~MethodInvocationMock() = default;

	virtual std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> getMatcher()= 0;

};

class finally {
private:
	std::function<void()> finallyClause;
	finally(const finally &);
	finally& operator=(const finally &);
public:
	explicit finally(std::function<void()> f) :
			finallyClause(f) {
	}

	~finally() {
		finallyClause();
	}
};

struct NoMoreRecordedBehaviorException: public std::exception {
};

template<typename R, typename ... arglist>
struct RecordedMethodBody: public MethodInvocationHandler<R, arglist...> {

	RecordedMethodBody(Method & method) :
			method(method) {
		clear();
	}

	void AppendDo(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> doMock = std::shared_ptr<Behavior<R, arglist...>> { new Repeat<R, arglist...>(method) };
		AppendDo(doMock);
	}

	void LastDo(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> doMock = std::shared_ptr<Behavior<R, arglist...>> { new Repeat<R, arglist...>(method) };
		LastDo(doMock);
	}

	void AppendDo(std::shared_ptr<Behavior<R, arglist...> > doMock) {
		append(doMock);
	}

	void LastDo(std::shared_ptr<Behavior<R, arglist...> > doMock) {
		append(doMock);
		behaviorMocks.pop_back();
	}

	R handleMethodInvocation(arglist&... args) override {
		std::shared_ptr<Behavior<R, arglist...>> behavior = behaviorMocks.front();
		std::function < void() > finallyClause = [&]()->void {
			if (behavior->isDone())
			behaviorMocks.erase(behaviorMocks.begin());
		};
		finally onExit(finallyClause);
		return behavior->invoke(args...);
	}

private:

	struct NoMoreRecordedBehavior: public Behavior<R, arglist...> {

		virtual ~NoMoreRecordedBehavior() = default;

		virtual R invoke(arglist&... args) override {
			throw NoMoreRecordedBehaviorException();
		}

		virtual bool isDone() override {
			return false;
		}
	};

	Method & method;

	void append(std::shared_ptr<Behavior<R, arglist...>> mock) {
		behaviorMocks.insert(behaviorMocks.end() - 1, mock);
	}

	void clear() {
		behaviorMocks.clear();
		auto doMock = std::shared_ptr<Behavior<R, arglist...>> { new NoMoreRecordedBehavior() };
		behaviorMocks.push_back(doMock);
	}

	std::vector<std::shared_ptr<Behavior<R, arglist...>>>behaviorMocks;
};

template<typename R, typename ... arglist>
struct MethodInvocationMockBase: public virtual MethodInvocationMock<R, arglist...> {

	virtual ~MethodInvocationMockBase() = default;

	MethodInvocationMockBase(const Method& method, std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> matcher,
			std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler) :
			method(method), matcher { matcher }, invocationHandler { invocationHandler } {
	}

	R handleMethodInvocation(arglist&... args) override {
		return invocationHandler->handleMethodInvocation(args...);
	}

	virtual bool matches(ActualInvocation<arglist...>& actualInvocation) {
		return matcher->matches(actualInvocation);
	}

	std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> getMatcher() override {
		return matcher;
	}

private:
	const Method& method;
	std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> matcher;
	std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler;
};

template<typename ... arglist>
struct ExpectedArgumentsInvocationMatcher: public ActualInvocation<arglist...>::Matcher {

	virtual ~ExpectedArgumentsInvocationMatcher() = default;

	ExpectedArgumentsInvocationMatcher(const arglist&... args) :
			expectedArguments(args...) {
	}

	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
		if (invocation.getActualMatcher().get() == this)
			return true;
		return matches(invocation.getActualArguments());
	}

private:
	virtual bool matches(const std::tuple<arglist...>& actualArgs) {
		return expectedArguments == actualArgs;
	}
	const std::tuple<arglist...> expectedArguments;
};

template<typename ... arglist>
struct UserDefinedInvocationMatcher: public ActualInvocation<arglist...>::Matcher {
	virtual ~UserDefinedInvocationMatcher() = default;

	UserDefinedInvocationMatcher(std::function<bool(arglist...)> matcher) :
			matcher { matcher } {
	}

	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
		if (invocation.getActualMatcher().get() == this)
			return true;
		return matches(invocation.getActualArguments());
	}
private:
	virtual bool matches(const std::tuple<arglist...>& actualArgs) {
		return invoke<arglist...>(matcher, std::tuple<arglist...> { actualArgs });
	}
	std::function<bool(arglist...)> matcher;
};

template<typename ... arglist>
struct DefaultInvocationMatcher: public ActualInvocation<arglist...>::Matcher {

	virtual ~DefaultInvocationMatcher() = default;

	DefaultInvocationMatcher() {
	}

	virtual bool matches(ActualInvocation<arglist...>& invocation) override {
		return matches(invocation.getActualArguments());
	}

private:
	virtual bool matches(const std::tuple<arglist...>& actualArgs) {
		return true;
	}
};

template<typename C, typename R, typename ... arglist>
class MethodMock: public virtual MethodInvocationHandler<R, arglist...>, public virtual ActualInvocationsSource {

	class MethodImpl: public Method {
		std::string _name;
	public:
		MethodImpl(std::string name) :
				_name(name) {
		}

		virtual std::string name() const override {
			return _name;
		}

		void setName(const std::string& name) {
			_name = name;
		}
	};

	MockObject<C>& mock;
	R (C::*vMethod)(arglist...);
	MethodImpl method;
	std::vector<std::shared_ptr<MethodInvocationMock<R, arglist...>>>methodInvocationMocks;
	std::vector<std::shared_ptr<ActualInvocation<arglist...>>> actualInvocations;

	std::shared_ptr<MethodInvocationMockBase<R, arglist...>> buildMethodInvocationMock(
			std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> invocationMatcher,
			std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler) {
		return std::shared_ptr<MethodInvocationMockBase<R, arglist...>> {
			new MethodInvocationMockBase<R, arglist...>(this->getMethod(), invocationMatcher,
					invocationHandler)};
	}

	std::shared_ptr<MethodInvocationMock<R, arglist...>> getMethodInvocationMockForActualArgs(ActualInvocation<arglist...>& invocation) {
		for (auto i = methodInvocationMocks.rbegin(); i != methodInvocationMocks.rend(); ++i) {
			if ((*i)->matches(invocation)) {
				return (*i);
			}
		}
		return nullptr;
	}

public:

	MethodMock(MockObject<C>& mock, R (C::*vMethod)(arglist...)) :
	mock(mock), vMethod(vMethod), method {typeid(vMethod).name()} {
	}

	virtual ~MethodMock() {
	}

	Method& getMethod() {
		return method;
	}

	void stubMethodInvocation(std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> invocationMatcher,
			std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler) {
		auto mock = buildMethodInvocationMock(invocationMatcher, invocationHandler);
		methodInvocationMocks.push_back(mock);
	}

	void clear() {
		methodInvocationMocks.clear();
	}

	R handleMethodInvocation(arglist&... args) override {
		int ordinal = invocationOrdinal++;
		auto actualInvoaction = std::shared_ptr<ActualInvocation<arglist...>> {new ActualInvocation<arglist...>(ordinal, this->getMethod(),
					args...)};
		auto methodInvocationMock = getMethodInvocationMockForActualArgs(*actualInvoaction);
		if (!methodInvocationMock) {
			UnexpectedMethodCallException e(this->method);
			FakeIt::log(e);
			throw e;
		}
		auto matcher = methodInvocationMock->getMatcher();
		actualInvoaction->setActualMatcher(matcher);
		actualInvocations.push_back(actualInvoaction);
		try {
			return methodInvocationMock->handleMethodInvocation(args...);
		} catch (NoMoreRecordedBehaviorException&) {
			UnexpectedMethodCallException e(this->method);
			FakeIt::log(e);
			throw e;
		}
	}

	std::vector<std::shared_ptr<ActualInvocation<arglist...>> > getActualInvocations(
			typename ActualInvocation<arglist...>::Matcher& matcher) {
		std::vector < std::shared_ptr<ActualInvocation<arglist...>> > result;
		for (auto actualInvocation : actualInvocations) {
			if (matcher.matches(*actualInvocation)) {
				result.push_back(actualInvocation);
			}
		}
		return result;
	}

	void getActualInvocations(std::unordered_set<Invocation*>& into) const {
		for (auto invocation : actualInvocations) {
			into.insert(invocation.get());
		}
	}

	void setMethodDetails(const std::string& mockName, const std::string& methodName) {
		const std::string fullName {mockName + "." + methodName};
		method.setName(fullName);
	}

};

}

#endif // MethodMock_h__
