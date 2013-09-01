#ifndef MethodMock_h__
#define MethodMock_h__
#include <vector>

template < typename R>
struct BehaviorMock
{
	virtual R play() = 0;
};

template < typename R>
struct ReturnMock : public BehaviorMock<R>
{
	ReturnMock(R r) : r(r){}
	virtual R play() override {
		return r;
	}
private:
	R r;
};

template < typename R>
struct DefaultReturnMock : public ReturnMock<R>
{
	DefaultReturnMock() : ReturnMock<R>(R{}){}
};

struct VoidMock : public BehaviorMock<void>
{
	virtual void play() override {
		return;
	}
};

template < typename R>
struct ThrowMock : public BehaviorMock<R>
{
	virtual R play() override {
		throw "error";
	}
};

template < typename R>
struct DoMock : public BehaviorMock<R>
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
	void append(BehaviorMock<R>* mock){
		behaviorMocks.push_back(mock);
	}

	void clear(){
		behaviorMocks.clear();
	}

	virtual bool matches(ActualInvocation<arglist...>& actualInvocation) = 0;

	R play(ActualInvocation<arglist...> & actualInvocation){
		BehaviorMock<R>* behavior = behaviorMocks.front();
		if (behaviorMocks.size() > 1)
			behaviorMocks.erase(behaviorMocks.begin());
		return behavior->play();
	}

private:
	std::vector<BehaviorMock<R>*> behaviorMocks;
};

template <typename R, typename... arglist>
struct InvocationMock : public InvocationMockBase<R, arglist...>
{
	InvocationMock(arglist... args) : expectedArguments(args...){}

	virtual bool matches(ActualInvocation<arglist...>& actualInvocation) override {
		return expectedArguments == actualInvocation.getArguments();
	}

private:
	std::tuple <arglist...> expectedArguments;
};

template <typename R, typename... arglist>
struct DefaultFunctionInvocationMock : public InvocationMockBase<R, arglist...>
{
	DefaultFunctionInvocationMock() {
		append(new DefaultReturnMock<R>());
	}

	virtual bool matches(ActualInvocation<arglist...>& actualInvocation){
		return true;
	}
};

template <typename... arglist>
struct DefaultProcedureInvocationMock : public InvocationMockBase<void, arglist...>
{
	DefaultProcedureInvocationMock() {
		append(new VoidMock());
	}

	virtual bool matches(ActualInvocation<arglist...>& actualInvocation){
		return true;
	}
};


template <typename R, typename... arglist>
struct MethodMock
{
	struct ActualIvocationBase{
	};

	struct ExpectedIvocationBase {
		virtual bool matches(ActualIvocationBase * actualInvocation) = 0;
	};

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

	std::vector<InvocationMockBase<R, arglist...>*>& getInvocationMocks(){
		return invocationMocks;
	}

	R play(ActualInvocation<arglist...> & actualInvocation){
		for (auto i = invocationMocks.rbegin(); i != invocationMocks.rend(); ++i) {
			if ((*i)->matches(actualInvocation)){
				return (*i)->play(actualInvocation);
			}
		}
		throw "error";
	}

private:
	std::vector<InvocationMockBase<R, arglist...>*> invocationMocks;
};

#endif // MethodMock_h__