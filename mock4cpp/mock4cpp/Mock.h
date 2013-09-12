#ifndef Mock_h__
#define Mock_h__

#include "../mockutils/DynamicProxy.h"
#include "../mock4cpp/ClousesImpl.h"

template <typename C>
struct Mock
{	

	Mock() : dynamicProxy(){
	}
	
	~Mock(){
	}
	
	C& get()
	{
		return dynamicProxy.get();
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<R(arglist...)>([](const arglist&... args)->R{return DefaultValue<R>::value(); }));
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<void(arglist...)>([](arglist... args)->void{return defualtProc<arglist...>(args...); }));
	}

	template <class MEMBER_TYPE, typename... arglist>
	void Stub(MEMBER_TYPE C::*member, const arglist&... ctorargs)
	{
		dynamicProxy.stubDataMember(member, ctorargs...);
	}

private:

	DynamicProxy<C> dynamicProxy;

	template<typename R, typename... arglist>
	static R defualtFunc(const arglist&...){
		static std::remove_reference<R>::type a{};
		return a;
//		return R{};
	}

	template<typename... arglist>
	static void defualtProc(const arglist&...){
	}

	template <typename R, typename... arglist>
	MethodMock<R, arglist...>* stubMethodIfNotStubbed(R(C::*vMethod)(arglist...), std::function<R(arglist...)> initialMethodBehavior){
		if (!dynamicProxy.isStubbed(vMethod)){
			auto methodMock = new MethodMock<R, arglist...>();
			methodMock->addMethodCall(new DefaultMethodCallMock<R, arglist...>(initialMethodBehavior));
			dynamicProxy.stubMethod(vMethod, methodMock);
		}
		MethodMock<R, arglist...> * methodMock = dynamicProxy.getMethodMock<MethodMock<R, arglist...> *>(vMethod);
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