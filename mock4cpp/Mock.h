#ifndef Mock_h__
#define Mock_h__

#include <functional>
#include "VirtualTable.h"
#include "Table.h"
#include "MethodMock.h"
#include "VirtualMethodOffsetLocator.h"
#include "Clouses.h"
#include "VirtualOffestSelector.h"
#include "utils.h"
#include "ClousesImpl.h"
#include <typeinfo>

struct UnmockedMethodException : public std::exception {
} unmockedMethodException;

template <typename C>
struct Mock
{	

	Mock() : vtable(10),methodMocks(10),name("Eran"){
		auto mptr = union_cast<void*>(&Mock::unmocked);
		for (unsigned int i = 0; i < vtable.getSize(); i++)
			vtable.setMethod(i, mptr);
	}
	
	~Mock(){
	}
	
	C& get()
	{
		return reinterpret_cast<C&>(*this);
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

	VirtualTable vtable;

	Table methodMocks;
	VirtualMethodOffsetLocator offsetLocator;
	std::string name;

	template <typename R, typename... arglist>
	struct MethodProxy {
		virtual unsigned int getOffset() = 0;
		virtual void * getProxy() = 0;
	};

	template <typename C, typename R, typename... arglist>
	struct MethodMockBase : public MethodMock <R, arglist...>
	{

		virtual unsigned int getOffset() override {
			return methodProxy->getOffset();
		}

		void *getProxy() override {
			return methodProxy->getProxy();
		}

		static MethodProxy<R, arglist...> * createMethodProxy(R(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<MethodMockBase<C, R, arglist...>::VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			return reinterpret_cast<MethodProxy<R, arglist...>*>(obj);
		}

		MethodMockBase(MethodProxy<R, arglist...> * methodProxy, BehaviorMock<R, arglist...> * defaultBehaviour) :
			methodProxy(methodProxy)
		{
			addInvocation(new DefaultInvocationMock<R, arglist...>(defaultBehaviour));
		}

	private:
		MethodProxy<R, arglist...> * methodProxy;

		template <unsigned int OFFSET>
		struct VirtualMethodProxy : public MethodProxy<R, arglist...> {

			unsigned int getOffset() override { return OFFSET; }

			void * getProxy() override { return union_cast<void *>(&VirtualMethodProxy::methodProxy); }

			R methodProxy(arglist... args){
				Mock<C> * m = union_cast<Mock<C> *>(this);
				MethodMock<R, arglist...> * methodMock = m->getMethodProxy<MethodMockBase<C, R, arglist...> *>(OFFSET);
				return methodMock->play(args...);
			}
		};
	};

	template <typename T>
	T getMethodMock(unsigned int offset){
		if (!isMocked(offset))
			return nullptr;
		return methodMocks.get<T>(offset);
	}

	template <typename T>
	T getMethodProxy(unsigned int offset){
		return methodMocks.get<T>(offset);
	}

	template <typename MethodMockType>
	MethodMockType& prepare(MethodMockType* methodMock)
	{
		auto offset = methodMock->getOffset();
		methodMocks.set(offset, methodMock);
		vtable.setMethod(methodMock->getOffset(), methodMock->getProxy());
		return *methodMock;
	}

	bool isMocked(unsigned int index){
		return vtable.getMethod(index) != union_cast<void*>(&Mock<C>::unmocked);
	}

	void unmocked(){
		Mock * m = this; // this should work
		throw unmockedMethodException;
	}

	template<typename R, typename... arglist>
	static R defualtFunc(arglist...){
		return R{};
	}

	template<typename... arglist>
	static void defualtProc(arglist...){
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...), std::function<R(arglist...)> def){
		auto methodProxy = MethodMockBase<C, R, arglist...>::createMethodProxy(vMethod);

		auto methodMock = getMethodMock<MethodMockBase<C, R, arglist...>*>(methodProxy->getOffset());
		if (methodMock == nullptr) {
			methodMock = new MethodMockBase<C, R, arglist...>(methodProxy, new DoMock<R, arglist...>(def));
			prepare(methodMock);
		}
		auto stubClouse = new StubFunctionClouseImpl<R, arglist...>(methodMock);
		return *stubClouse;
	}

	template <typename R, typename... arglist>
	StubFunctionClouse<R, arglist...>& Stub(R(C::*vMethod)(arglist...), R(*defaultMethod)(arglist...)){
		return Stub(vMethod, std::function <R(arglist...)>(defaultMethod));
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...), std::function<void(arglist...)> def){
		auto methodProxy = MethodMockBase<C, void, arglist...>::createMethodProxy(vMethod);

		auto methodMock = getMethodMock<MethodMockBase<C, void, arglist...>*>(methodProxy->getOffset());
		if (methodMock == nullptr)
			methodMock = new MethodMockBase<C, void, arglist...>(methodProxy, new DoMock<void, arglist...>(def));

		auto stubClouse = new StubProcedureClouseImpl<arglist...>(methodMock);
		prepare(methodMock);
		return *stubClouse;
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...), void(*defaultMethod)(arglist...)){
		return Stub(vMethod, std::function <void(arglist...)>(defaultMethod));
	}


};

#endif // Mock_h__