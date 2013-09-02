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
		auto methodMock = MethodMockBase<C, R, arglist...>::createFunc(this, vMethod);
		auto stubClouse = new StubFunctionClouseImpl<R, arglist...>(methodMock);
		prepare(methodMock);
		return *stubClouse;
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...)){
		auto methodMock = MethodMockBase<C, void,arglist...>::createProc(this, vMethod);
		auto stubClouse = new StubProcedureClouseImpl<arglist...>(methodMock);
		prepare(methodMock);
		return *stubClouse;
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

		virtual unsigned int getOffset() {
			return methodProxy->getOffset();
		}

		void *getProxy(){
			return methodProxy->getProxy();
		}

		static MethodMock<R, arglist...> * createFunc(Mock<C> * mock, R(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<MethodMockBase<C, R, arglist...>::VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			auto methodProxy = reinterpret_cast<MethodProxy<R, arglist...>*>(obj);
			return new MethodMockBase<C, R, arglist...>(methodProxy, new DefaultReturnMock<R>());
		}

		static MethodMock<R, arglist...> * createProc(Mock<C> * mock, R(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<MethodMockBase<C, R, arglist...>::VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			auto methodProxy = reinterpret_cast<MethodProxy<R, arglist...>*>(obj);
			return new MethodMockBase<C, R, arglist...>(methodProxy, new VoidMock());
		}

	private:
		MethodProxy<R, arglist...> * methodProxy;

		MethodMockBase(MethodProxy<R, arglist...> * methodProxy, BehaviorMock<R> * defaultBehaviour) :
			methodProxy(methodProxy)
		{
			append(new DefaultInvocationMock<R, arglist...>(defaultBehaviour));
		}

		template <unsigned int OFFSET>
		struct VirtualMethodProxy : public MethodProxy<R, arglist...>{

			unsigned int getOffset() override { return OFFSET; }

			void * getProxy() override { return union_cast<void *>(&VirtualMethodProxy::methodProxy); }

			R methodProxy(arglist... args){
				Mock<C> * m = union_cast<Mock<C> *>(this);
				MethodMock<R, arglist...> * methodMock = m->getMethodMock<MethodMockBase<C, R, arglist...> *>(OFFSET);
				ActualInvocation<arglist...>* actualInvocation = new ActualInvocation<arglist...>(args...);
				return methodMock->play(*actualInvocation);
			}
		};
	};

	template <typename T>
	T getMethodMock(unsigned int offset){
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

	bool isMocked(int index){
		return vtable.getMethod(index) != &unmocked;
	}

	void unmocked(){
		Mock * m = this; // this should work
		throw unmockedMethodException;
	}
};

#endif // Mock_h__