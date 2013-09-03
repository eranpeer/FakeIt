#ifndef MethodMock_h__
#define MethodMock_h__
#include <vector>

template < typename R, typename... arglist>
struct BehaviorMock
{
	virtual R play() = 0;
};

template < typename R, typename... arglist>
struct ReturnMock : public BehaviorMock<R, arglist...>
{
	ReturnMock(R r) : r(r){}
	virtual R play() override {
		return r;
	}
private:
	R r;
};

template < typename R, typename... arglist>
struct DefaultReturnMock : public ReturnMock<R, arglist...>
{
	DefaultReturnMock() : ReturnMock<R,arglist...>(R{}){}
};

template <typename... arglist>
struct VoidMock : public BehaviorMock<void,arglist...>
{
	virtual void play() override {
		return;
	}
};

template < typename R, typename... arglist>
struct ThrowMock : public BehaviorMock<R, arglist...>
{
	virtual R play() override {
		throw "error";
	}
};

template <typename R, typename... arglist>
struct DoMock : public BehaviorMock<R, arglist...>
{
};

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

template <typename R, typename... arglist>
struct InvocationMockBase
{
	void append(BehaviorMock<R, arglist...>* mock){
		behaviorMocks.push_back(mock);
	}

	void clear(){
		behaviorMocks.clear();
	}

	virtual bool matchesActual(ActualInvocation<arglist...>& actualInvocation) {
		return matchesActual(actualInvocation.getArguments());
	}
	
	virtual bool matchesActual(std::tuple<arglist...> &args) = 0;

	virtual bool matchesExpected(std::tuple<arglist...> &args) = 0;

	R play(ActualInvocation<arglist...> & actualInvocation){
		BehaviorMock<R, arglist...>* behavior = behaviorMocks.front();
		if (behaviorMocks.size() > 1)
			behaviorMocks.erase(behaviorMocks.begin());
		return behavior->play();
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

	R play(ActualInvocation<arglist...> & actualInvocation){
		for (auto i = invocationMocks.rbegin(); i != invocationMocks.rend(); ++i) {
			if ((*i)->matchesActual(actualInvocation)){
				return (*i)->play(actualInvocation);
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