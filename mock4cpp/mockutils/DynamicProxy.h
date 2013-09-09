#ifndef DynamicProxy_h__
#define DynamicProxy_h__

#include <functional>
#include <new>  
#include "../mockutils/MethodProxy.h"
#include "../mockutils/VirtualTable.h"
#include "../mockutils/Table.h"
#include "../mockutils/VirtualOffestSelector.h"
#include "../mockutils/utils.h"

struct UnmockedMethodException : public std::exception {
};

template <typename C>
struct DynamicProxy
{

	DynamicProxy() : vtable(), methodMocks(vtable.getSize()){
		auto mptr = union_cast<void*>(&DynamicProxy::unmocked);
		for (unsigned int i = 0; i < vtable.getSize(); i++) {
			vtable.setMethod(i, mptr);
		}
		for (int i = 0; i < sizeof(instanceMembersArea); i++)
			instanceMembersArea[i] = (char)0;
	}

	~DynamicProxy(){
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
				DynamicProxy<C> * mo = union_cast<DynamicProxy<C> *>(this);
				MethodInvocationHandler<R, arglist...> * methodMock = mo->getMethodMock<MethodInvocationHandler<R, arglist...> *>(OFFSET);
				return methodMock->handleMethodInvocation(args...);
			}
		};
	};

	VirtualTable<10> vtable;

	// Here we alloc too many bytes since sizeof(C) includes the pointer to the virtual table.
	// Should be sizeof(C) - ptr_size.
	// No harm is done if we alloc more space for data but don't use it.
	char instanceMembersArea[sizeof(C)];
	
	Table methodMocks;

	void unmocked(){
		DynamicProxy * m = this; // this should work
		throw UnmockedMethodException{};
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
#endif // DynamicProxy_h__