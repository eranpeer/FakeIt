#ifndef Mock_h__
#define Mock_h__

#include "MockObject.h"
#include "ClousesImpl.h"

template <typename C>
struct Mock
{	

	Mock() : mo(){
	}
	
	~Mock(){
	}
	
	C& get()
	{
		return mo.get();
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<R(arglist...)>([](arglist... args)->R{return defualtFunc<R,arglist...>(args...); }));
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<void(arglist...)>([](arglist... args)->void{return defualtProc<arglist...>(args...); }));
	}

private:

	MockObject<C> mo;

	template<typename R, typename... arglist>
	static R defualtFunc(arglist...){
		return R{};
	}

	template<typename... arglist>
	static void defualtProc(arglist...){
	}

	template <typename R, typename... arglist>
	MethodMock<R, arglist...>* stubMethodIfNotStubbed(R(C::*vMethod)(arglist...), std::function<R(arglist...)> initialMethodBehavior){
		if (!mo.isStubbed(vMethod)){
			auto methodMock = new MethodMock<R, arglist...>();
			methodMock->addMethodCall(new DefaultMethodCallMockMock<R, arglist...>(initialMethodBehavior));
			mo.stubMethod(vMethod, methodMock);
		}
		MethodMock<R, arglist...> * methodMock = mo.getMethodMock<MethodMock<R, arglist...> *>(vMethod);
		return methodMock;
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...), std::function<R(arglist...)> initialMethodBehavior){
		auto methodMock = stubMethodIfNotStubbed(vMethod, initialMethodBehavior);
		auto stubClouse = new StubFunctionClouseImpl<R, arglist...>(methodMock);
		return *stubClouse;
	}
	
	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...), R(*initialMethodBehavior)(arglist...)){
		return Stub(vMethod, std::function <R(arglist...)>(initialMethodBehavior));
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...), void(*initialMethodBehavior)(arglist...)){
		return Stub(vMethod, std::function <void(arglist...)>(initialMethodBehavior));
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...), std::function<void(arglist...)> initialMethodBehavior){
		auto methodMock = stubMethodIfNotStubbed(vMethod, initialMethodBehavior);
		auto stubClouse = new StubProcedureClouseImpl<arglist...>(methodMock);
		return *stubClouse;
	}
};

#endif // Mock_h__