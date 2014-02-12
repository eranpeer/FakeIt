#ifndef DynamicProxy_h__
#define DynamicProxy_h__

#include <functional>
#include <type_traits>
#include <vector>
#include <array>
#include <new>

#ifdef _WIN32
#include "mockutils/mscpp/VirtualTable.h"
#else
#include "mockutils/gcc/VirtualTable.h"
#endif

#include "mockutils/MethodProxy.h"
#include "mockutils/VirtualOffestSelector.h"
#include "mockutils/union_cast.h"
#include "mockutils/MethodInvocationHandler.h"

template <typename C>
struct DynamicProxy {
	static_assert(std::is_polymorphic<C>::value, "DynamicProxy requires a polymorphic type");

	DynamicProxy(std::function<void()> unmockedMethodCallHandler) :
			vtable(), methodMocks(), unmockedMethodCallHandler { unmockedMethodCallHandler } {
		auto mptr = union_cast<void*>(&DynamicProxy::unmocked);
		for (unsigned int i = 0; i < vtable.getSize(); i++) {
			vtable.setMethod(i, mptr);
		}
		initializeDataMembersArea();
	}

	~DynamicProxy() {
	}

	C& get() {
		return reinterpret_cast<C&>(*this);
	}

	template<typename R, typename ... arglist>
	void stubMethod(R (C::*vMethod)(arglist...), std::shared_ptr<MethodInvocationHandler<R, arglist...>> methodInvocationHandler) {
		std::shared_ptr<MethodProxy<R, arglist...>> methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		bind(methodProxy, methodInvocationHandler);
	}

	template<typename R, typename ... arglist>
	bool isStubbed(R (C::*vMethod)(arglist...)) {
		std::shared_ptr<MethodProxy<R, arglist...>> methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		std::shared_ptr<Destructable> ptr = methodMocks[methodProxy->getOffset()];
		return ptr.get() != nullptr;
	}

	template<typename R, typename ... arglist>
	Destructable * getMethodMock(R (C::*vMethod)(arglist...)) {
		std::shared_ptr<MethodProxy<R, arglist...>> methodProxy = MethodProxyCreator<R, arglist...>::createMethodProxy(vMethod);
		std::shared_ptr<Destructable> ptr = methodMocks[methodProxy->getOffset()];
		return ptr.get();
	}

	template<typename DATA_TYPE, typename ... arglist>
	void stubDataMember(DATA_TYPE C::*member, const arglist&... initargs) {
		DATA_TYPE C::*theMember = (DATA_TYPE C::*) member;
		C& mock = get();
		DATA_TYPE *memberPtr = &(mock.*theMember);
		members.push_back(
				std::shared_ptr<DataMemeberWrapper<DATA_TYPE, arglist...>> { new DataMemeberWrapper<DATA_TYPE, arglist...>(memberPtr,
						initargs...) });
	}

	template<typename DATA_TYPE>
	void getMethodMocks(std::vector<DATA_TYPE>& into) const {
		for (std::shared_ptr<Destructable> ptr :methodMocks){
			DATA_TYPE p = dynamic_cast<DATA_TYPE>(ptr.get());
			if (p)
				into.push_back(p);
		}
	}

private:

	template<typename R, typename ... arglist>
	class MethodProxyCreator {
	private:

		template<unsigned int OFFSET>
		struct VirtualMethodProxy: public MethodProxy<R, arglist...> {
			unsigned int getOffset() override {
				return OFFSET;
			}

			void * getProxy() override {
				return union_cast<void *>(&VirtualMethodProxy::methodProxy);
			}
		private:
			R methodProxy(arglist ... args) {
				DynamicProxy<C> * dynamicProxy = union_cast<DynamicProxy<C> *>(this);
				MethodInvocationHandler<R, arglist...> * methodMock = dynamicProxy->getMethodMock<MethodInvocationHandler<R, arglist...> *>(
						OFFSET);
				return methodMock->handleMethodInvocation(args...);
			}
		};

	public:

		static std::shared_ptr<MethodProxy<R, arglist...>> createMethodProxy(R (C::*vMethod)(arglist...)) {
			static VirtualOffsetSelector<VirtualMethodProxy> offsetSelctor;
			auto* obj = offsetSelctor.create(vMethod);
			auto rv = reinterpret_cast<MethodProxy<R, arglist...>*>(obj);
			return std::shared_ptr<MethodProxy<R, arglist...>> { rv };
		}

	};

	template<typename DATA_TYPE, typename ... arglist>
	class DataMemeberWrapper: public Destructable {
	private:
		DATA_TYPE *dataMember;
	public:
		DataMemeberWrapper(DATA_TYPE *dataMember, const arglist&... initargs) :
				dataMember(dataMember) {
			new (dataMember) DATA_TYPE { initargs ... };
		}
		~DataMemeberWrapper() {
			dataMember->~DATA_TYPE();
		}
	};

	VirtualTable<30,C> vtable;

	// Here we alloc too many bytes since sizeof(C) includes the pointer to the virtual table.
	// Should be sizeof(C) - ptr_size.
	// No harm is done if we alloc more space for data but don't use it.
	char instanceMembersArea[sizeof(C)];

	std::array<std::shared_ptr<Destructable>, 30> methodMocks;
	std::vector<std::shared_ptr<Destructable>> members;
	std::function<void()> unmockedMethodCallHandler;

	void unmocked() {
		//DynamicProxy * m = this; // this should work
		unmockedMethodCallHandler();
	}

	template<typename R, typename ... arglist>
	static R defualtFunc(arglist...) {
		return R { };
	}

	template<typename ... arglist>
	static void defualtProc(arglist...) {
	}

	template<typename R, typename ... arglist>
	void bind(std::shared_ptr<MethodProxy<R, arglist...>> methodProxy,
			std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler) {
		auto offset = methodProxy->getOffset();
		vtable.setMethod(offset, methodProxy->getProxy());
		methodMocks[offset] = invocationHandler;
	}

	void initializeDataMembersArea() {
		for (int i = 0; i < sizeof(instanceMembersArea); i++)
			instanceMembersArea[i] = (char) 0;
	}

	template<typename DATA_TYPE>
	DATA_TYPE getMethodMock(unsigned int offset) {
		std::shared_ptr<Destructable> ptr = methodMocks[offset];
		return dynamic_cast<DATA_TYPE>(ptr.get());
	}

};
#endif // DynamicProxy_h__
