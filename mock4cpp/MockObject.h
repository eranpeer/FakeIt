#ifndef MockObject_h__
#define MockObject_h__

#include <functional>
#include "MethodProxy.h"
#include "VirtualTable.h"
#include "Table.h"
#include "VirtualOffestSelector.h"
#include "utils.h"

struct UnmockedMethodException : public std::exception {
} unmockedMethodException;

template <typename C>
struct MockObject
{

	MockObject() : vtable(10), methodMocks(10){
		auto mptr = union_cast<void*>(&MockObject::unmocked);
		for (unsigned int i = 0; i < vtable.getSize(); i++) {
			vtable.setMethod(i, mptr);
		}
	}

	~MockObject(){
	}

	C& get()
	{
		return reinterpret_cast<C&>(*this);
	}

	template <typename R, typename... arglist>
	void stubMethod(R(C::*vMethod)(arglist...), MethodInvocationHandler<R, arglist...> * methodMock){
		MethodProxy<R,arglist...> * methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		MethodInvocationHandler<R, arglist...>* methodInvocationHandler = methodMock;
		bind(methodProxy, methodInvocationHandler);
	}

	template <typename C, typename R, typename... arglist>
	bool isStubbed(R(C::*vMethod)(arglist...)){
		MethodProxy<R, arglist...> * methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		return methodMocks.get<void *>(methodProxy->getOffset()) != nullptr;
	}

	template <typename MOCK, typename C, typename R, typename... arglist>
	MOCK getMethodMock(R(C::*vMethod)(arglist...)){
		MethodProxy<R, arglist...> * methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		return methodMocks.get<MOCK>(methodProxy->getOffset());
	}

private:

	template <typename R, typename... arglist>
	struct MethodProxyCreator
	{
		static MethodProxy<R, arglist...> * createMethodProxy(R(C::*vMethod)(arglist...)){
			VirtualOffsetSelector<VirtualMethodProxy> c;
			void * obj = c.create(vMethod);
			return reinterpret_cast<MethodProxy<R, arglist...>*>(obj);
		}
	private:

		template <unsigned int OFFSET>
		struct VirtualMethodProxy : public MethodProxy<R, arglist...> {

			unsigned int getOffset() override { return OFFSET; }

			void * getProxy() override { return union_cast<void *>(&VirtualMethodProxy::methodProxy); }

		private:
			R methodProxy(arglist... args){
				MockObject<C> * mo = union_cast<MockObject<C> *>(this);
				MethodInvocationHandler<R, arglist...> * methodMock = mo->getMethodMock<MethodInvocationHandler<R, arglist...> *>(OFFSET);
				return methodMock->handleMethodInvocation(args...);
			}
		};
	};

	VirtualTable vtable;
	Table methodMocks;


	void unmocked(){
		MockObject * m = this; // this should work
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
	void bind(MethodProxy<R, arglist...> * methodProxy, MethodInvocationHandler<R, arglist...> * invocationHandler)
	{
		auto offset = methodProxy->getOffset();
		vtable.setMethod(offset, methodProxy->getProxy());
		methodMocks.set(offset, invocationHandler);
	}

	template <typename T>
	T getMethodMock(unsigned int offset){
		return methodMocks.get<T>(offset);
	}

};
#endif // MockObject_h__