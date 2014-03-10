#ifndef MethodMock_h__
#define MethodMock_h__

#include <vector>
#include <functional>
#include <atomic>
#include <tuple>

#include "mockutils/TupleDispatcher.h"
#include "mockutils/DefaultValue.hpp"

#include "fakeit/DomainObjects.h"
#include "fakeit/ActualInvocation.h"
#include "fakeit/Exceptions.h"

namespace fakeit {

static std::atomic_int invocationOrdinal;

template<typename R, typename ... arglist>
struct BehaviorMock {
	virtual R invoke(arglist&... args) = 0;
	virtual bool isDone() = 0;
};

template<typename R, typename ... arglist>
struct DoMock: public BehaviorMock<R, arglist...> {
	DoMock(std::function<R(arglist&...)> f) :
			f(f), times(1) {
	}

	DoMock(std::function<R(arglist&...)> f, long times) :
			f(f), times(times) {
	}

	virtual R invoke(arglist&... args) override {
		times--;
		return f(args...);
	}

	virtual bool isDone() override {
		return times == 0;
	}
private:
	std::function<R(arglist&...)> f;
	long times;
};

template<typename R, typename ... arglist>
struct DoForeverMock: public BehaviorMock<R, arglist...> {
	DoForeverMock(std::function<R(arglist&...)> f) :
			f(f) {
	}
	virtual R invoke(arglist&... args) override {
		return f(args...);
	}

	virtual bool isDone() override {
		return false;
	}
private:
	std::function<R(arglist&...)> f;
};

template<typename R, typename ... arglist>
struct EndMock: public BehaviorMock<R, arglist...> {
	virtual R invoke(arglist&... args) override {
		throw UnmockedMethodCallException();
	}

	virtual bool isDone() override {
		return false;
	}
};

template<typename R, typename ... arglist>
struct InitialMock: public BehaviorMock<R, arglist...> {
	virtual R invoke(arglist&... args) override {
		return DefaultValue::value<R>();
	}

	virtual bool isDone() override {
		return false;
	}
};

template<typename R, typename ... arglist>
struct MethodInvocationMock: public ActualInvocation<arglist...>::Matcher, public MethodInvocationHandler<R, arglist...> {

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

template<typename R, typename ... arglist>
struct RecordedMethodBody: public MethodInvocationHandler<R, arglist...> {

	RecordedMethodBody() {
		auto initialMock = std::shared_ptr<BehaviorMock<R, arglist...>> { new InitialMock<R, arglist...>() };
		behaviorMocks.push_back(initialMock);
	}

	void AppendDo(std::function<R(arglist...)> method) {
		std::shared_ptr<BehaviorMock<R, arglist...>> doMock = std::shared_ptr<BehaviorMock<R, arglist...>> { new DoMock<R, arglist...>(
				method) };
		AppendDo(doMock);
	}

	void LastDo(std::function<R(arglist...)> method) {
		std::shared_ptr<BehaviorMock<R, arglist...>> doMock = std::shared_ptr<BehaviorMock<R, arglist...>> { new DoMock<R, arglist...>(
				method) };
		LastDo(doMock);
	}

	void AppendDo(std::shared_ptr<BehaviorMock<R, arglist...> > doMock) {
		if (isFirstAppend()) {
			clear();
		}
		append(doMock);
	}

	void LastDo(std::shared_ptr<BehaviorMock<R, arglist...> > doMock) {
		append(doMock);
		behaviorMocks.pop_back();
	}

	R handleMethodInvocation(arglist&... args) override {
		std::shared_ptr<BehaviorMock<R, arglist...>> behavior = behaviorMocks.front();
		std::function < void() > finallyClause = [&]()->void {
			if (behavior->isDone())
			behaviorMocks.erase(behaviorMocks.begin());
		};
		finally onExit(finallyClause);
		return behavior->invoke(args...);
	}

private:
	void append(std::shared_ptr<BehaviorMock<R, arglist...>> mock) {
		behaviorMocks.insert(behaviorMocks.end() - 1, mock);
	}

	void clear() {
		behaviorMocks.clear();
		auto doMock = std::shared_ptr<BehaviorMock<R, arglist...>> { new EndMock<R, arglist...>() };
		behaviorMocks.push_back(doMock);
	}

	bool isFirstAppend() {
		return dynamic_cast<InitialMock<R, arglist...>*>(behaviorMocks.back().get()) != nullptr;
	}

	std::vector<std::shared_ptr<BehaviorMock<R, arglist...>>>behaviorMocks;
};

template<typename R, typename ... arglist>
struct MethodInvocationMockBase: public virtual MethodInvocationMock<R, arglist...> {

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
struct MethodMock: public virtual Method, public virtual MethodInvocationHandler<R, arglist...>, public virtual ActualInvocationsSource {
	MethodMock(MockObject& mock, R (C::*vMethod)(arglist...)) :
			mock(mock), vMethod(vMethod) {
	}

	virtual ~MethodMock() {
	}

	std::string getMethodName() const override {
		return typeid(vMethod).name();
			}

	virtual MockObject& getMockObject() override {
		return mock;
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
		auto actualInvoaction = std::shared_ptr<ActualInvocation<arglist...>> { new ActualInvocation<arglist...>(ordinal, *this,
				args...) };
		auto methodInvocationMock = getMethodInvocationMockForActualArgs(*actualInvoaction);
		if (!methodInvocationMock) {
			throw UnmockedMethodCallException();
		}
		auto matcher = methodInvocationMock->getMatcher();
		actualInvoaction->setActualMatcher(matcher);
		actualInvocations.push_back(actualInvoaction);
		return methodInvocationMock->handleMethodInvocation(args...);
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

	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const {
		for (auto invocation : actualInvocations) {
			into.insert(invocation.get());
		}
	}
private:

	MockObject& mock;
	R (C::*vMethod)(arglist...);
	std::vector<std::shared_ptr<MethodInvocationMock<R, arglist...>>>methodInvocationMocks;
	std::vector<std::shared_ptr<ActualInvocation<arglist...>>> actualInvocations;

	std::shared_ptr<MethodInvocationMockBase<R, arglist...>> buildMethodInvocationMock(
			std::shared_ptr<typename ActualInvocation<arglist...>::Matcher> invocationMatcher,
			std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler) {
		return std::shared_ptr<MethodInvocationMockBase<R, arglist...>> {new MethodInvocationMockBase<R, arglist...>(*this, invocationMatcher,
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

};

}

#endif // MethodMock_h__
