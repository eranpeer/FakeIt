#ifndef MockObject_h__
#define MockObject_h__

#include <functional>
#include "VirtualTable.h"
#include "Table.h"
#include "MethodMock.h"
#include "VirtualOffestSelector.h"
#include "utils.h"

struct UnmockedMethodException : public std::exception {
} unmockedMethodException;

template <typename C>
struct MockObject
{

	MockObject() : vtable(10), methodMocks(10){
		auto mptr = union_cast<void*>(&MockObject::unmocked);
		for (unsigned int i = 0; i < vtable.getSize(); i++)
			vtable.setMethod(i, mptr);
	}

	~MockObject(){
	}

	C& get()
	{
		return reinterpret_cast<C&>(*this);
	}

	template <typename R, typename... arglist>
	MethodMock<R, arglist...>* stubMethod(R(C::*vMethod)(arglist...), std::function<R(arglist...)> initialMethodBehavior){
		MethodProxy<R,arglist...> * methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		auto methodMock = getMethodMock<MethodMock<R, arglist...>*>(methodProxy->getOffset());
		if (methodMock == nullptr) {
			MethodInvocationHandler<R, arglist...>* methodInvocationHandler = 
				new InnerMethodMock<C, R, arglist...>(initialMethodBehavior);
			bind(methodProxy, methodInvocationHandler);
		}
		return methodMock;
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
				MethodInvocationHandler<R, arglist...> * methodMock = mo->getMethodProxy<MethodInvocationHandler<R, arglist...> *>(OFFSET);
				return methodMock->handleMethodInvocation(args...);
			}
		};
	};

	VirtualTable vtable;
	Table methodMocks;

	template <typename C, typename R, typename... arglist>
	struct InnerMethodMock : public MethodMock <R, arglist...>
	{
		InnerMethodMock(std::function<R(arglist...)> initialMethodBehavior) 
		{	
			addMethodCall(new DefaultMethodCallMockMock<R, arglist...>(initialMethodBehavior));
		}
	};


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

	template <typename T>
	T getMethodProxy(unsigned int offset){
		return methodMocks.get<T>(offset);
	}

	bool isMocked(unsigned int index){
		return vtable.getMethod(index) != union_cast<void*>(&MockObject<C>::unmocked);
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
		if (!isMocked(offset))
			return nullptr;
		return methodMocks.get<T>(offset);
	}

};
#endif // MockObject_h__