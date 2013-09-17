#ifndef Mock_h__
#define Mock_h__

#include <type_traits>
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
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...) const){
		auto methodWithoutConst = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConst);
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...) volatile){
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...) const volatile){
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<R(arglist...)>([](const arglist&... args)->R&{return DefaultValue::value<R>(); }));
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<void(arglist...)>([](const arglist&... args)->void{}));
	}

	template <class MEMBER_TYPE, typename... arglist>
	void Stub(MEMBER_TYPE C::*member, const arglist&... ctorargs)
	{
		dynamicProxy.stubDataMember(member, ctorargs...);
	}

private:

// 	template< class T >
// 	struct is_member_function_helper : std::false_type {};
// 
// 	template< class R>
// 	struct is_member_function_helper<R C::*> : std::true_type{};
// 
// 	template< class T >
// 	struct is_member_function : is_member_function_helper<
// 		typename std::remove_cv<T>::type
// 	> {};
// 
// 
// 
// 	template< class T >
// 	struct is_member_proc_helper : std::false_type {};
// 
// 	template< class R>
// 	struct is_member_proc_helper<R C::*> : std::is_void<R>{};
// 
// 	template< class T >
// 	struct is_member_proc : is_member_proc_helper<
// 		typename std::remove_cv<T>::type
// 	> {};


	DynamicProxy<C> dynamicProxy;

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