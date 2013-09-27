#ifndef Mock_h__
#define Mock_h__

#include <type_traits>
#include "../mockutils/DynamicProxy.h"
#include "../mock4cpp/ClousesImpl.h"

using namespace mock4cpp::stub_clouses;

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

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...) const){
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template < typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...) volatile){
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...) const volatile){
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...)) {
		return Stub(vMethod, std::function<R(arglist...)>([](const arglist&... args)->R&{return DefaultValue::value<R>(); }));
	}

	//		

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	StubProcedureClouse<void, arglist...>& Stub(R(C::*vMethod)(arglist...) const){
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	StubProcedureClouse<void, arglist...>& Stub(R(C::*vMethod)(arglist...) volatile){
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	StubProcedureClouse<void, arglist...>& Stub(R(C::*vMethod)(arglist...) const volatile){
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return Stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	StubProcedureClouse<void, arglist...>& Stub(R(C::*vMethod)(arglist...)){
		return Stub(vMethod, std::function<void(arglist...)>([](const arglist&... args)->void{}));
	}

	template <class DM, typename... arglist
		, class = typename std::enable_if<std::is_member_object_pointer<DM>::value>::type
	>
	void Stub(DM member, const arglist&... ctorargs)
	{
		stubDataMember(member, ctorargs...);
	}

	template <typename H, typename... M
		, class = typename std::enable_if<std::is_member_function_pointer<H>::value>::type
	>
	void Stub(const H head, M... tail) 
	{
		Stub(head);
		Stub(tail...);
	}

	template <
		typename MEMBER
		, class = typename std::enable_if<std::is_member_function_pointer<MEMBER>::value>::type
	>
	auto operator [](MEMBER member) -> decltype(Stub(member)) {
		return Stub(member);
	}

private:

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
	StubProcedureClouse<void,arglist...>& Stub(void(C::*vMethod)(arglist...), void(*initialMethodBehavior)(arglist...)){
		return Stub(vMethod, std::function <void(arglist...)>(initialMethodBehavior));
	}

	template <typename... arglist>
	StubProcedureClouse<void, arglist...>& Stub(void(C::*vMethod)(arglist...), std::function<void(arglist...)> initialMethodBehavior){
		auto methodMock = stubMethodIfNotStubbed(vMethod, initialMethodBehavior);
		auto stubClouse = new StubProcedureClouseImpl<void, arglist...>(methodMock);
		return *stubClouse;
	}

	void Stub(){}

	template <class MEMBER_TYPE, typename... arglist>
	void stubDataMember(MEMBER_TYPE C::*member, const arglist&... ctorargs)
	{
		dynamicProxy.stubDataMember(member, ctorargs...);
	}

};

#endif // Mock_h__