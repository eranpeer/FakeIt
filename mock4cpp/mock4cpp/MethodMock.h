#ifndef MethodMock_h__
#define MethodMock_h__

#include <vector>
#include <functional>
#include "InvocationMatcher.h"
#include "MockRepository.h"
#include "ActualInvocation.h"

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

		virtual bool matchesActual(const arglist&... args) override = 0;

		virtual bool matchesExpected(const arglist&... args) override = 0;

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

		virtual bool matchesActual(const arglist&... args) override {
			return expectedArguments == std::tuple<arglist...>(args...);
		}

		virtual bool matchesExpected(const arglist&... args) override {
			return expectedArguments == std::tuple<arglist...>(args...);
		}

	private:
		const std::tuple <arglist...> expectedArguments;
	};

	template <typename R, typename... arglist>
	struct DefaultInvocationMock : public MethodInvocationMockBase<R, arglist...>
	{
		DefaultInvocationMock(std::function<R(arglist...)> methodBehavior) {
			appendDo(methodBehavior);
		}

		virtual bool matchesActual(const arglist&... args) override {
			return true;
		}

		virtual bool matchesExpected(const arglist&... args) override {
			return false;
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
			actualInvocations.push_back(actualInvoaction);
			auto * methodInvocationMock = getMethodInvocationMockForActualArgs(args...);
			return methodInvocationMock->handleMethodInvocation(args...);
		}

		std::vector<ActualInvocation<arglist...> *> getActualInvocations(const arglist&... expectedArgs) {
			std::vector<ActualInvocation<arglist...> *> result;
			for each (auto* actualInvocation in actualInvocations)
			{
				if (actualInvocation->matches(expectedArgs...)){
					result.push_back(actualInvocation);
				}
			}
			return result;
		}

	private:

		MockBase& mock;
		std::vector<MethodInvocationMock<R, arglist...>*> methodInvocationMocks;
		std::vector<ActualInvocation<arglist...>*> actualInvocations;

		MethodInvocationMock<R, arglist...> * getMethodInvocationMockForExpectedArgs(const arglist&... expectedArgs){
			for (auto i = methodInvocationMocks.rbegin(); i != methodInvocationMocks.rend(); ++i) {
				if ((*i)->matchesExpected(expectedArgs...)){
					return (*i);
				}
			}
			return nullptr;
		}

		MethodInvocationMock<R, arglist...>* getMethodInvocationMockForActualArgs(const arglist&... args) {
			for (auto i = methodInvocationMocks.rbegin(); i != methodInvocationMocks.rend(); ++i) {
				if ((*i)->matchesActual(args...)){
					return (*i);
				}
			}
			return nullptr;
		}

	};

}
#endif // MethodMock_h__