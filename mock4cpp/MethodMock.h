#ifndef MethodMock_h__
#define MethodMock_h__
#include <vector>

template <typename... arglist>
struct ActualInvocation
{
	ActualInvocation(arglist... args) : arguments(args...){}

	std::tuple <arglist...>& getArguments(){
		return arguments;
	}

private:
	std::tuple <arglist...> arguments;
};

template < typename R, typename... arglist>
struct BehaviorMock
{
	virtual R play(arglist... args) = 0;
};

template <typename R, typename... arglist>
struct DoMock : public BehaviorMock<R, arglist...>
{
	DoMock(std::function<R(arglist...)> f):f(f){}
	virtual R play(arglist... args) override {
		return f(args...);
	}
private:
	std::function<R(arglist...)> f;
};

template <typename R, typename... arglist>
struct InvocationMockBase
{
	void append(BehaviorMock<R, arglist...>* mock){
		behaviorMocks.push_back(mock);
	}

	void clear(){
		behaviorMocks.clear();
	}

	virtual bool matchesActual(arglist... args) {
		return matchesActual(std::tuple<arglist...>(args...));
	}
	
	virtual bool matchesActual(std::tuple<arglist...> &args) = 0;

	virtual bool matchesExpected(std::tuple<arglist...> &args) = 0;

	R play(arglist... args){
		BehaviorMock<R, arglist...>* behavior = behaviorMocks.front();
		if (behaviorMocks.size() > 1)
			behaviorMocks.erase(behaviorMocks.begin());
		return behavior->play(args...);
	}

private:
	std::vector<BehaviorMock<R, arglist...>*> behaviorMocks;
};

template <typename R, typename... arglist>
struct InvocationMock : public InvocationMockBase<R, arglist...>
{
	InvocationMock(arglist... args) : expectedArguments(args...){}

	virtual bool matchesActual(std::tuple<arglist...> &args) override {
		return expectedArguments == args;
	}

	virtual bool matchesExpected(std::tuple<arglist...> &args) override {
		return expectedArguments == args;
	}

private:
	std::tuple <arglist...> expectedArguments;
};

template <typename R, typename... arglist>
struct DefaultInvocationMock : public InvocationMockBase<R, arglist...>
{
	DefaultInvocationMock(BehaviorMock<R, arglist...> * defaultBehavior) {
		append(defaultBehavior);
	}

	virtual bool matchesActual(std::tuple<arglist...>& args) override {
		return true;
	}

	virtual bool matchesExpected(std::tuple<arglist...>& args) override {
		return false;
	}

};

template <typename R, typename... arglist>
struct MethodMock
{

	~MethodMock(){}

	virtual unsigned int getOffset() = 0;
	virtual void * getProxy() = 0;

	void append(InvocationMockBase<R, arglist...> * mock){
		invocationMocks.push_back(mock);
 	}

	void clear(){
		invocationMocks.clear();
	}

	InvocationMockBase<R, arglist...>* first(){
		return invocationMocks.front();
	}

	InvocationMockBase<R, arglist...>* last(){
		return invocationMocks.back();
	}

	std::vector<InvocationMockBase<R, arglist...>*>& getInvocationMocks(){
		return invocationMocks;
	}

	R play(arglist... args){
		for (auto i = invocationMocks.rbegin(); i != invocationMocks.rend(); ++i) {
			if ((*i)->matchesActual(args...)){
				return (*i)->play(args...);
			}
		}
		throw "error";
	}

	InvocationMockBase<R, arglist...> * getInvocationMock(arglist... expectedArgs){
		std::tuple<arglist...> tuple(expectedArgs...);
		for (auto i = invocationMocks.rbegin(); i != invocationMocks.rend(); ++i) {
			if ((*i)->matchesExpected(tuple)){
				return (*i);
			}
		}
		return nullptr;
	}

private:
	std::vector<InvocationMockBase<R, arglist...>*> invocationMocks;
};

#endif // MethodMock_h__