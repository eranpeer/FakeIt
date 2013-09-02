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

template <typename R, typename... arglist>
struct MethodProxy {
	virtual unsigned int getOffset() = 0;
	virtual void * getProxy() = 0;
};

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
		auto methodMock = FunctionMock<R, arglist...>::create(this, vMethod);
		auto stubClouse = new StubFunctionClouseImpl<R, arglist...>(methodMock);
		prepare(methodMock);
		return *stubClouse;
	}

	template <typename... arglist>
	StubProcedureClouse<arglist...>& Stub(void(C::*vMethod)(arglist...)){
		auto methodMock = ProcedureMock<arglist...>::create(this, vMethod);
		auto stubClouse = new StubProcedureeClouseImpl<arglist...>(methodMock);
		prepare(methodMock);
		return *stubClouse;
	}

	template <typename MethodMockType>
	MethodMockType& prepare(MethodMockType* methodMock)
	{
		auto offset = methodMock->getOffset();
		methodMocks.set(offset, methodMock);
		vtable.setMethod(methodMock->getOffset(), methodMock->getProxy());
		return *methodMock;
	}

	template <typename T>
	T getMethodMock(unsigned int offset){
		return methodMocks.get<T>(offset);
	}

private:
	VirtualTable vtable;

	Table methodMocks;
	VirtualMethodOffsetLocator offsetLocator;
	std::string name;

	template <typename R, typename... arglist>
	struct MethodMockBase : public MethodMock <R, arglist...>
	{

		virtual unsigned int getOffset() {
			return methodProxy->getOffset();
		}

		void *getProxy(){
			return methodProxy->getProxy();
		}

		static MethodMockBase<R, arglist...> * create(Mock<C> * mock, R(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			auto methodProxy = reinterpret_cast<MethodMockBase<R, arglist...>*>(obj);
			return  new FunctionMock<R, arglist...>(methodProxy);
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
				Mock * m = union_cast<Mock *>(this);
				MethodMockBase<R, arglist...> * methodMock = m->getMethodMock<MethodMockBase<R, arglist...> *>(OFFSET);
				ActualInvocation<arglist...>* actualInvocation = new ActualInvocation<arglist...>(args...);
				return methodMock->play(*actualInvocation);
			}
		};
	};



	template <typename R, typename... arglist>
	struct FunctionMock : public MethodMock <R, arglist...>
	{

		virtual unsigned int getOffset() {
			return methodProxy->getOffset();
		}

		void *getProxy(){
			return methodProxy->getProxy();
		}

		static FunctionMock<R, arglist...> * create(Mock<C> * mock, R(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			auto methodProxy = reinterpret_cast<MethodProxy<R,arglist...>*>(obj);
			return  new FunctionMock<R, arglist...>(methodProxy);
		}

	private:
		MethodProxy<R, arglist...> * methodProxy;

		FunctionMock(MethodProxy<R,arglist...> * methodProxy) :
			methodProxy(methodProxy)
		{
			append(new DefaultInvocationMock<R, arglist...>(new DefaultReturnMock<R>()));
		}

		template <unsigned int OFFSET>
		struct VirtualMethodProxy : public MethodProxy<R, arglist...>{

			unsigned int getOffset() override { return OFFSET; }

			void * getProxy() override { return union_cast<void *>(&VirtualMethodProxy::methodProxy); }

			R methodProxy(arglist... args){
				Mock * m = union_cast<Mock *>(this);
				FunctionMock<R, arglist...> * methodMock = m->getMethodMock<FunctionMock<R, arglist...> *>(OFFSET);
				ActualInvocation<arglist...>* actualInvocation = new ActualInvocation<arglist...>(args...);
				return methodMock->play(*actualInvocation);
			}
		};

	};

	template <typename... arglist>
	struct ProcedureMock : public MethodMock<void, arglist...>
	{

		ProcedureMock(MethodProxy<void, arglist...> * methodProxy) :
			methodProxy(methodProxy)
		{
			append(new DefaultInvocationMock<void, arglist...>(new VoidMock()));
		}

		virtual unsigned int getOffset() {
			return methodProxy->getOffset();
		}

		void *getProxy(){
			return methodProxy->getProxy();
		}

		static ProcedureMock<arglist...> * create(Mock * mock, void(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			auto methodProxy = reinterpret_cast<MethodProxy<void,arglist...>*>(obj);
			return  new ProcedureMock<arglist...>(methodProxy);
		}

	private:
		MethodProxy<void, arglist...> * methodProxy;

		template <unsigned int OFFSET>
		struct VirtualMethodProxy : public MethodProxy<void, arglist...>{

			unsigned int getOffset() override { return OFFSET; }

			void * getProxy() override { return union_cast<void *>(&VirtualMethodProxy::methodProxy); }

			void methodProxy(arglist... args){
				Mock * m = union_cast<Mock *>(this);
				ProcedureMock<arglist...> * methodMock = m->getMethodMock<ProcedureMock<arglist...> *>(OFFSET);
				ActualInvocation<arglist...>* actualInvocation = new ActualInvocation<arglist...>(args...);
				return methodMock->play(*actualInvocation);
			}
		};

	};

	bool isMocked(int index){
		return vtable.getMethod(index) != &unmocked;
	}

	void unmocked(){
		Mock * m = this; // this should work
		throw unmockedMethodException;
	}
};

#endif // Mock_h__