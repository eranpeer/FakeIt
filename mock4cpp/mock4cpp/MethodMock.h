#ifndef MethodMock_h__
#define MethodMock_h__

#include <vector>
#include <functional>

namespace mock4cpp {

	template <typename... arglist>
	struct ActualInvocation
	{
		ActualInvocation(const arglist&... args) : arguments(args...){}
		std::tuple <arglist...>& getArguments(){
			return arguments;
		}
	private:
		std::tuple <arglist...> arguments;
	};

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
	struct MethodInvocationMock
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
	struct SimpleMethodInvocationMock : public MethodInvocationMock<R, arglist...>
	{
		SimpleMethodInvocationMock(const arglist&... args) : expectedArguments(args...)
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
	struct DefaultMethodInvocationMock : public MethodInvocationMock<R, arglist...>
	{
		DefaultMethodInvocationMock(std::function<R(arglist...)> methodBehavior) {
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

		void addMethodCall(MethodInvocationMock<R, arglist...> * mock){
			methodInvocationMocks.push_back(mock);
		}

		void clear(){
			methodInvocationMocks.clear();
		}

		MethodInvocationMock<R, arglist...>* last(){
			return methodInvocationMocks.back();
		}

		R handleMethodInvocation(const arglist&... args) override {
			auto * methodInvocationMock = getMethodInvocationMockForActualArgs(args...);
			return methodInvocationMock->handleMethodInvocation(args...);
		}

		MethodInvocationMock<R, arglist...> * stubMethodCall(const arglist&... args){
			MethodInvocationMock<R, arglist...> * methodInvocationMock = getMethodInvocationMockForExpectedArgs(args...);
			if (methodInvocationMock == nullptr) {
				methodInvocationMock = new SimpleMethodInvocationMock<R, arglist...>(args...);
				addMethodCall(methodInvocationMock);
			}
			return methodInvocationMock;
		}

	private:
		std::vector<MethodInvocationMock<R, arglist...>*> methodInvocationMocks;

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
			// should not get here since the default will always match an actual method call.
			return nullptr;
		}

	};

}
#endif // MethodMock_h__