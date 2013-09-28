#ifndef MethodMock_h__
#define MethodMock_h__

#include <vector>
#include <functional>

// template <typename... arglist>
// struct ActualInvocation
// {
// 	ActualInvocation(arglist... args) : arguments(args...){}
// 
// 	std::tuple <arglist...>& getArguments(){
// 		return arguments;
// 	}
// 
// private:
// 	std::tuple <arglist...> arguments;
// };

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
	struct MethodCallMock
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

		virtual bool matchesActual(const arglist&... args) = 0;

		virtual bool matchesExpected(const arglist&... args) = 0;

		R handleMethodInvocation(const arglist&... args){
			BehaviorMock<R, arglist...>* behavior = behaviorMocks.front();
			if (behaviorMocks.size() > 1)
				behaviorMocks.erase(behaviorMocks.begin());
			return behavior->invoke(args...);
		}

	private:
		std::vector<BehaviorMock<R, arglist...>*> behaviorMocks;
	};

	template <typename R, typename... arglist>
	struct SimpleMethodCallMock : public MethodCallMock<R, arglist...>
	{
		SimpleMethodCallMock(const arglist&... args) : expectedArguments(args...)
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
	struct DefaultMethodCallMock : public MethodCallMock<R, arglist...>
	{
		DefaultMethodCallMock(std::function<R(arglist...)> methodBehavior) {
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

		virtual ~MethodMock() override {}

		void addMethodCall(MethodCallMock<R, arglist...> * mock){
			methodCallMocks.push_back(mock);
		}

		void clear(){
			methodCallMocks.clear();
		}

		MethodCallMock<R, arglist...>* last(){
			return methodCallMocks.back();
		}

		R handleMethodInvocation(const arglist&... args) override {
			return getMethodCallMock(args...)->handleMethodInvocation(args...);
		}

		MethodCallMock<R, arglist...> * stubMethodCall(const arglist&... args){
			MethodCallMock<R, arglist...> * invocationMock = getInvocationMock(args...);
			if (invocationMock == nullptr) {
				invocationMock = new SimpleMethodCallMock<R, arglist...>(args...);
				addMethodCall(invocationMock);
			}
			return invocationMock;
		}

	private:
		std::vector<MethodCallMock<R, arglist...>*> methodCallMocks;

		std::vector<MethodCallMock<R, arglist...>*>& getMethodCallMocks(){
			return methodCallMocks;
		}

		MethodCallMock<R, arglist...> * getInvocationMock(const arglist&... expectedArgs){
			for (auto i = methodCallMocks.rbegin(); i != methodCallMocks.rend(); ++i) {
				if ((*i)->matchesExpected(expectedArgs...)){
					return (*i);
				}
			}
			return nullptr;
		}

		MethodCallMock<R, arglist...>* getMethodCallMock(const arglist&... args) {
			for (auto i = methodCallMocks.rbegin(); i != methodCallMocks.rend(); ++i) {
				if ((*i)->matchesActual(args...)){
					return (*i);
				}
			}
			return nullptr; // should not get here since the default will always match a method call.
		}

	};

}
#endif // MethodMock_h__