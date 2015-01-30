/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DynamicProxy_h__
#define DynamicProxy_h__

#include <functional>
#include <type_traits>
#include <vector>
#include <array>
#include <new>

#ifdef _MSC_VER
#include "mockutils/mscpp/VirtualTable.hpp"
#else
#include "mockutils/gcc/VirtualTable.hpp"
#endif

#include "mockutils/union_cast.hpp"
#include "mockutils/MethodInvocationHandler.hpp"
#include "mockutils/VTUtils.hpp"
#include "mockutils/FakeObject.hpp"

namespace fakeit {

template<typename C, typename ... baseclasses>
struct DynamicProxy {

	static_assert(std::is_polymorphic<C>::value, "DynamicProxy requires a polymorphic type");

	DynamicProxy(C& instance) :
			instance(instance), originalVtHandle(VirtualTable<C, baseclasses...>::getVTable(instance).createHandle()), methodMocks() {
		cloneVt.copyFrom(originalVtHandle.restore());
		cloneVt.setCookie(0, this);
		getFake().setVirtualTable(cloneVt);
	}

	void detach()
	{
		getFake().setVirtualTable(originalVtHandle.restore());
	}

	~DynamicProxy() {
		cloneVt.dispose();
	}

	C& get() {
		return instance;
	}

	void Reset() {
		methodMocks = {{}};
		members = {};
		cloneVt.copyFrom(originalVtHandle.restore());
	}

	template<typename R, typename ... arglist>
	void stubMethod(R (C::*vMethod)(arglist...), MethodInvocationHandler<R, arglist...>* methodInvocationHandler) {
		MethodProxyCreator<R, arglist...> creator;
        bindMethod(creator.createMethodProxy(vMethod), methodInvocationHandler);
	}

	void stubDtor(MethodInvocationHandler<void>* methodInvocationHandler) {
		MethodProxyCreator<void> creator;
        bindDtor(creator.createDtorProxy(), methodInvocationHandler);
	}

	template<typename R, typename ... arglist>
	bool isMethodStubbed(R (C::*vMethod)(arglist...)) {
		unsigned int offset = MethodProxyCreator<R, arglist...>::getOffset(vMethod);
		return isBinded(offset);
	}

	bool isDtorStubbed() {
		unsigned int offset = VTUtils::getDestructorOffset<C>();
		return isBinded((unsigned int) offset);
	}

	template<typename R, typename ... arglist>
	Destructable * getMethodMock(R (C::*vMethod)(arglist...)) {
		auto offset = MethodProxyCreator<R, arglist...>::getOffset(vMethod);
		std::shared_ptr<Destructable> ptr = methodMocks[offset];
		return ptr.get();
	}

	Destructable * getDtorMock() {
		auto offset = VTUtils::getDestructorOffset<C>();
		std::shared_ptr<Destructable> ptr = methodMocks[offset];
		return ptr.get();
	}

	template<typename DATA_TYPE, typename ... arglist>
	void stubDataMember(DATA_TYPE C::*member, const arglist&... initargs) {
		DATA_TYPE C::*theMember = (DATA_TYPE C::*) member;
		C& mock = get();
		DATA_TYPE *memberPtr = &(mock.*theMember);
		members.push_back(
				std::shared_ptr<DataMemeberWrapper<DATA_TYPE, arglist...>> {new DataMemeberWrapper<DATA_TYPE, arglist...>(memberPtr,
							initargs...)});
	}

	template<typename DATA_TYPE>
	void getMethodMocks(std::vector<DATA_TYPE>& into) const {
		for (std::shared_ptr<Destructable> ptr : methodMocks) {
			DATA_TYPE p = dynamic_cast<DATA_TYPE>(ptr.get());
			if (p) {
				into.push_back(p);
			}
		}
	}

	VirtualTable<C, baseclasses...>& getOriginalVT() {
		VirtualTable<C, baseclasses...>& vt = originalVtHandle.restore();
		return vt;
	}

private:

	struct MethodProxy {

		MethodProxy(unsigned int offset, void * vMethod) :
				_vMethod(vMethod), _offset(offset) {
		}

		unsigned int getOffset() const {
			return _offset;
		}

		void * getProxy() const {
			return union_cast<void *>(_vMethod);
		}

	private:
		void * _vMethod;
		unsigned int _offset;
	};

	static DynamicProxy<C, baseclasses...> * getDynamicProxy(void * methodProxy) {
		C* instance = (C*)methodProxy;
		VirtualTable<C,baseclasses...> & vt = VirtualTable<C,baseclasses...>::getVTable(*instance);
		DynamicProxy<C, baseclasses...> * dynamicProxy = (DynamicProxy<C, baseclasses...> *)vt.getCookie(0);
		return dynamicProxy;
	}

	template<typename R, typename ... arglist>
	class MethodProxyCreator {
	private:

		R methodProxy(unsigned int index, arglist& ... args) {
			DynamicProxy<C, baseclasses...> * dynamicProxy = getDynamicProxy(this);
			MethodInvocationHandler<R, arglist...> * methodMock = dynamicProxy->getMethodMock<MethodInvocationHandler<R, arglist...> *>(index);
			return methodMock->handleMethodInvocation(args...);
		}

        template<int offset>
        R methodProxyX(arglist ... args) {
            return methodProxy(offset, args...);
        }

		MethodProxy newMethodProxy(unsigned int offset, R(MethodProxyCreator::*vMethod)(arglist...)) {
			return MethodProxy(offset, union_cast<void *>(vMethod));
		}

	public:

		MethodProxy createMethodProxy(unsigned int offset) {
			R(MethodProxyCreator::*vMethod)(arglist...) = 0;
			switch (offset) {
				case 0:
				vMethod = &MethodProxyCreator::methodProxyX<0>;
				break;
				case 1:
				vMethod = &MethodProxyCreator::methodProxyX<1>;
				break;
				case 2:
				vMethod = &MethodProxyCreator::methodProxyX<2>;
				break;
				case 3:
				vMethod = &MethodProxyCreator::methodProxyX<3>;
				break;
				case 4:
				vMethod = &MethodProxyCreator::methodProxyX<4>;
				break;
				case 5:
				vMethod = &MethodProxyCreator::methodProxyX<5>;
				break;
				case 6:
				vMethod = &MethodProxyCreator::methodProxyX<6>;
				break;
				case 7:
				vMethod = &MethodProxyCreator::methodProxyX<7>;
				break;
				case 8:
				vMethod = &MethodProxyCreator::methodProxyX<8>;
				break;
				case 9:
				vMethod = &MethodProxyCreator::methodProxyX<9>;
				break;
				case 10:
				vMethod = &MethodProxyCreator::methodProxyX<10>;
				break;
				case 11:
				vMethod = &MethodProxyCreator::methodProxyX<11>;
				break;
				case 12:
				vMethod = &MethodProxyCreator::methodProxyX<12>;
				break;
				case 13:
				vMethod = &MethodProxyCreator::methodProxyX<13>;
				break;
				case 14:
				vMethod = &MethodProxyCreator::methodProxyX<14>;
				break;
				case 15:
				vMethod = &MethodProxyCreator::methodProxyX<15>;
				break;
				case 16:
				vMethod = &MethodProxyCreator::methodProxyX<16>;
				break;
				case 17:
				vMethod = &MethodProxyCreator::methodProxyX<17>;
				break;
				case 18:
				vMethod = &MethodProxyCreator::methodProxyX<18>;
				break;
				case 19:
				vMethod = &MethodProxyCreator::methodProxyX<19>;
				break;
				case 20:
				vMethod = &MethodProxyCreator::methodProxyX<20>;
				break;
				case 21:
				vMethod = &MethodProxyCreator::methodProxyX<21>;
				break;
				case 22:
				vMethod = &MethodProxyCreator::methodProxyX<22>;
				break;
				case 23:
				vMethod = &MethodProxyCreator::methodProxyX<23>;
				break;
				case 24:
				vMethod = &MethodProxyCreator::methodProxyX<24>;
				break;
				case 25:
				vMethod = &MethodProxyCreator::methodProxyX<25>;
				break;
				case 26:
				vMethod = &MethodProxyCreator::methodProxyX<26>;
				break;
				case 27:
				vMethod = &MethodProxyCreator::methodProxyX<27>;
				break;
				case 28:
				vMethod = &MethodProxyCreator::methodProxyX<28>;
				break;
				case 29:
				vMethod = &MethodProxyCreator::methodProxyX<29>;
				break;
				case 30:
				vMethod = &MethodProxyCreator::methodProxyX<30>;
				break;
				case 31:
				vMethod = &MethodProxyCreator::methodProxyX<31>;
				break;
				case 32:
				vMethod = &MethodProxyCreator::methodProxyX<32>;
				break;
				case 33:
				vMethod = &MethodProxyCreator::methodProxyX<33>;
				break;
				case 34:
				vMethod = &MethodProxyCreator::methodProxyX<34>;
				break;
				case 35:
				vMethod = &MethodProxyCreator::methodProxyX<35>;
				break;
				case 36:
				vMethod = &MethodProxyCreator::methodProxyX<36>;
				break;
				case 37:
				vMethod = &MethodProxyCreator::methodProxyX<37>;
				break;
				case 38:
				vMethod = &MethodProxyCreator::methodProxyX<38>;
				break;
				case 39:
				vMethod = &MethodProxyCreator::methodProxyX<39>;
				break;
				case 40:
				vMethod = &MethodProxyCreator::methodProxyX<40>;
				break;
				case 41:
				vMethod = &MethodProxyCreator::methodProxyX<41>;
				break;
				case 42:
				vMethod = &MethodProxyCreator::methodProxyX<42>;
				break;
				case 43:
				vMethod = &MethodProxyCreator::methodProxyX<43>;
				break;
				case 44:
				vMethod = &MethodProxyCreator::methodProxyX<44>;
				break;
				case 45:
				vMethod = &MethodProxyCreator::methodProxyX<45>;
				break;
				case 46:
				vMethod = &MethodProxyCreator::methodProxyX<46>;
				break;
				case 47:
				vMethod = &MethodProxyCreator::methodProxyX<47>;
				break;
				case 48:
				vMethod = &MethodProxyCreator::methodProxyX<48>;
				break;
				case 49:
				vMethod = &MethodProxyCreator::methodProxyX<49>;
				break;
			}
			return newMethodProxy(offset,vMethod);
		}

		MethodProxy createMethodProxy(R (C::*vMethod)(arglist...)) {
			auto offset = getOffset(vMethod);
			return createMethodProxy(offset);
		}

		MethodProxy createDtorProxy() {
			auto offset = VTUtils::getDestructorOffset<C>();
			return createMethodProxy(offset);
		}

		static unsigned int getOffset(R (C::*vMethod)(arglist...)) {
			return VTUtils::getOffset(vMethod);
		}

	};

	template<typename DATA_TYPE, typename ... arglist>
	class DataMemeberWrapper: public Destructable {
	private:
		DATA_TYPE *dataMember;
	public:
		DataMemeberWrapper(DATA_TYPE *dataMember, const arglist&... initargs) :
		dataMember(dataMember) {
			new (dataMember) DATA_TYPE {initargs ...};
		}
		~DataMemeberWrapper() {
			dataMember->~DATA_TYPE();
		}
	};

	static_assert(sizeof(C) == sizeof(FakeObject<C,baseclasses...>), "This is a problem");

	C& instance;
	typename VirtualTable<C, baseclasses...>::Handle originalVtHandle; // avoid delete!! this is the original!
	VirtualTable<C, baseclasses...> cloneVt;//
	std::array<std::shared_ptr<Destructable>, 50> methodMocks;
	std::vector<std::shared_ptr<Destructable>> members;

	FakeObject<C,baseclasses...>& getFake() {
		return reinterpret_cast<FakeObject<C,baseclasses...>&>(instance);
	}

	template<typename R, typename ... arglist>
	static R defualtFunc(arglist...) {
		return R {};
	}

	template<typename ... arglist>
	static void defualtProc(arglist...) {
	}

	template<typename R, typename ... arglist>
	void bindMethod(const MethodProxy &methodProxy, MethodInvocationHandler<R, arglist...> *invocationHandler) {
		auto offset = methodProxy.getOffset();
		getFake().setMethod(offset, methodProxy.getProxy());
		Destructable * destructable = invocationHandler;
		methodMocks[offset].reset(destructable);
	}

	void bindDtor(const MethodProxy &methodProxy, MethodInvocationHandler<void> *invocationHandler) {
        auto offset = methodProxy.getOffset();
		getFake().setDtor(methodProxy.getProxy());
        Destructable * destructable = invocationHandler;
        methodMocks[offset].reset(destructable);
    }

    template<typename DATA_TYPE>
	DATA_TYPE getMethodMock(unsigned int offset) {
		std::shared_ptr<Destructable> ptr = methodMocks[offset];
		return dynamic_cast<DATA_TYPE>(ptr.get());
	}

	template<typename BaseClass>
	void checkMultipleInheritance() {
		C* ptr = (C*) (unsigned int) 1;
		BaseClass* basePtr = ptr;
		int delta = (unsigned long) basePtr - (unsigned long) ptr;
		if (delta > 0) {
			// base class does not start on same position as derived class.
			// this is multiple inheritance.
			throw std::invalid_argument(std::string("multiple inheritance is not supported"));
		}
	}

	bool isBinded(unsigned int offset) {
		std::shared_ptr<Destructable> ptr = methodMocks[offset];
		return ptr.get() != nullptr;
	}

};
}
#endif // DynamicProxy_h__
