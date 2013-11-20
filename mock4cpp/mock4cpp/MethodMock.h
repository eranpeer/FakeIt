#ifndef MethodMock_h__
#define MethodMock_h__

#include <vector>
#include <functional>
#include "InvocationMatcher.h"
#include "MockRepository.h"
#include "ActualInvocation.h"
#include "TupleDispatcher.h"

namespace mock4cpp {

	template < typename R, typename... arglist>
	struct BehaviorMock
	{
		virtual R invoke(const arglist&... args) = 0;
	};

	template <typename R, typename... arglist>
	struct DoMock : public BehaviorMock<R, arglist...>
	{
		DoMock(std::function<R(arglist...)> f) :f(f){}
		virtual R invoke(const arglist&... args) override {
			return f(args...);
		}
	private:
		std::function<R(arglist...)> f;
	};

	template <typename R, typename... arglist>
	struct MethodInvocationMock : public InvocationMatcher <arglist...>, public MethodInvocationHandler <R, arglist...>
	{

	};

	template <typename R, typename... arglist>
	struct MethodInvocationMockBase : public MethodInvocationMock <R, arglist...>
	{
		void append(BehaviorMock<R, arglist...>* mock){
			behaviorMocks.push_back(mock);
		}

		void appendDo(std::function<R(arglist...)> method) {
			auto doMock = new DoMock<R, arglist...>(method);
			append(doMock);
		}

		void clear(){
			behaviorMocks.clear();
		}

		virtual bool matches(ActualInvocation<arglist...>& actualArgs) override = 0;

		R handleMethodInvocation(const arglist&... args) override {
			BehaviorMock<R, arglist...>* behavior = behaviorMocks.front();
			if (behaviorMocks.size() > 1)
				behaviorMocks.erase(behaviorMocks.begin());
			return behavior->invoke(args...);
		}

	private:
		std::vector<BehaviorMock<R, arglist...>*> behaviorMocks;
	};

	template <typename R, typename... arglist>
	struct ExpectedInvocationMock : public MethodInvocationMockBase<R, arglist...>
	{
		ExpectedInvocationMock(const arglist&... args) : expectedArguments(args...)
		{
		}

		virtual bool matches(ActualInvocation<arglist...>& invocation) override {
			return matches(invocation.getActualArguments());
		}
	private:
		virtual bool matches(const std::tuple<arglist...>& actualArgs)  {
			return expectedArguments == actualArgs;
		}
		const std::tuple <arglist...> expectedArguments;
	};

	template <typename R, typename... arglist>
	struct ExpectedInvocationMock2 : public MethodInvocationMockBase<R, arglist...>
	{
		ExpectedInvocationMock2(const arglist&... matchers) : argumentsMatchers(args...)
		{
		}

		virtual bool matches(ActualInvocation<arglist...>& invocation) override {
			return matches(invocation.getActualArguments());
		}
	private:
		virtual bool matches(const std::tuple<arglist...>& actualArgs)  {
			return argumentsMatchers == actualArgs;
		}
		const std::tuple <arglist...> argumentsMatchers;
	};

	template <typename R, typename... arglist>
	struct MatchingInvocationMock : public MethodInvocationMockBase<R, arglist...>
	{
		MatchingInvocationMock(std::function<bool(arglist...)> matcher) : matcher{ matcher }
		{
		}

		virtual bool matches(ActualInvocation<arglist...>& invocation) override {
			return matches(invocation.getActualArguments());
		}
	private:
		virtual bool matches(const std::tuple<arglist...>& actualArgs)  {
			return invoke<arglist...>(matcher, std::tuple<arglist...>{actualArgs});
		}
		std::function<bool(arglist...)> matcher;
	};

	template <typename R, typename... arglist>
	struct DefaultInvocationMock : public MethodInvocationMockBase<R, arglist...>
	{
		DefaultInvocationMock(std::function<R(arglist...)> methodBehavior) {
			appendDo(methodBehavior);
		}

		virtual bool matches(ActualInvocation<arglist...>& invocation) override {
			return matches(invocation.getActualArguments());
		}

	private:
		virtual bool matches(const std::tuple<arglist...>& actualArgs)  {
			return true;
		}
	};

	template <typename R, typename... arglist>
	struct MethodMock : public MethodInvocationHandler <R, arglist...>
	{
		MethodMock(MockBase& mock):mock{mock} {}
		
		virtual ~MethodMock() override {}

		void stubMethodInvocation(MethodInvocationMock<R, arglist...> * methodInvocationMock){
			methodInvocationMocks.push_back(methodInvocationMock);
		}

		void clear(){
			methodInvocationMocks.clear();
		}

		R handleMethodInvocation(const arglist&... args) override {
			auto * actualInvoaction = new ActualInvocation<arglist...>(args...);
			actualInvocations.push_back(std::shared_ptr < ActualInvocation < arglist... >> {actualInvoaction});
			auto * methodInvocationMock = getMethodInvocationMockForActualArgs(*actualInvoaction);
			return methodInvocationMock->handleMethodInvocation(args...);
		}

		std::vector < std::shared_ptr < ActualInvocation<arglist... >> > getActualInvocations(const arglist&... expectedArgs) {
			std::vector < std::shared_ptr < ActualInvocation<arglist... >> > result;
			for each (auto actualInvocation in actualInvocations)
			{
				if (actualInvocation->matches(expectedArgs...)){
					result.push_back(actualInvocation);
				}
			}
			return result;
		}

		std::vector < std::shared_ptr < ActualInvocation<arglist... >> > getActualInvocations(InvocationMatcher<arglist...>& matcher) {
			std::vector < std::shared_ptr < ActualInvocation<arglist... >> > result;
			for each (auto actualInvocation in actualInvocations)
			{
				if (matcher.matches(*actualInvocation)){
					result.push_back(actualInvocation);
				}
			}
			return result;
		}

	private:

		MockBase& mock;
		std::vector<MethodInvocationMock<R, arglist...>*> methodInvocationMocks;
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>> actualInvocations;

		MethodInvocationMock<R, arglist...>* getMethodInvocationMockForActualArgs(ActualInvocation<arglist...>& invocation) {
			for (auto i = methodInvocationMocks.rbegin(); i != methodInvocationMocks.rend(); ++i) {
				if ((*i)->matches(invocation)){
					return (*i);
				}
			}
			return nullptr;
		}

	};

}
#endif // MethodMock_h__