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

#ifdef _WIN32
#include "mockutils/mscpp/VirtualTable.hpp"
#else
#include "mockutils/gcc/VirtualTable.hpp"
#endif

#include "mockutils/MethodProxy.hpp"
#include "mockutils/union_cast.hpp"
#include "mockutils/MethodInvocationHandler.hpp"
#include "mockutils/VTUtils.hpp"
#include "mockutils/FakeObject.hpp"

namespace fakeit {

template<typename C, typename ... baseclasses>
struct DynamicProxy {

	static_assert(std::is_polymorphic<C>::value, "DynamicProxy requires a polymorphic type");

	DynamicProxy(C& instance) :
			instance(instance), originalVT(VirtualTable<C, baseclasses...>::getVTable(instance).createHandle()), methodMocks() {
		cloneVt.copyFrom(originalVT.restore());
		cloneVt.setCookie(this);
		getFake().setVirtualTable(cloneVt);
	}

	~DynamicProxy() {
		cloneVt.dispose();
	}

	C& get() {
		return instance;
	}

	void Reset() {
		methodMocks = {};
		members = {};
		cloneVt.copyFrom(originalVT.restore());
	}

	template<typename R, typename ... arglist>
	void stubMethod(R (C::*vMethod)(arglist...), std::shared_ptr<MethodInvocationHandler<R, arglist...>> methodInvocationHandler) {
		MethodProxyCreator<R, arglist...> creator;
		std::shared_ptr<MethodProxy<R, arglist...>> methodProxy = creator.createMethodProxy(vMethod);
		bind(methodProxy, methodInvocationHandler);
	}

	template<typename R, typename ... arglist>
	bool isStubbed(R (C::*vMethod)(arglist...)) {
		auto offset = MethodProxyCreator<R, arglist...>::getOffset(vMethod);
		std::shared_ptr<Destructable> ptr = methodMocks[offset];
		return ptr.get() != nullptr;
	}

	template<typename R, typename ... arglist>
	Destructable * getMethodMock(R (C::*vMethod)(arglist...)) {
		auto offset = MethodProxyCreator<R, arglist...>::getOffset(vMethod);
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
		VirtualTable<C, baseclasses...>& vt = originalVT.restore();
		return vt;
	}

private:

	static DynamicProxy<C, baseclasses...> * getDynamicProxy(void * methodProxy) {
		C* instance = (C*)methodProxy;
		VirtualTable<C,baseclasses...> & vt = VirtualTable<C,baseclasses...>::getVTable(*instance);
		DynamicProxy<C, baseclasses...> * dynamicProxy = (DynamicProxy<C, baseclasses...> *)vt.getCookie();
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

		R methodProxy0(arglist ... args) {
			return methodProxy(0, args...);
		}
		R methodProxy1(arglist ... args) {
			return methodProxy(1, args...);
		}
		R methodProxy2(arglist ... args) {
			return methodProxy(2, args...);
		}
		R methodProxy3(arglist ... args) {
			return methodProxy(3, args...);
		}
		R methodProxy4(arglist ... args) {
			return methodProxy(4, args...);
		}
		R methodProxy5(arglist ... args) {
			return methodProxy(5, args...);
		}
		R methodProxy6(arglist ... args) {
			return methodProxy(6, args...);
		}
		R methodProxy7(arglist ... args) {
			return methodProxy(7, args...);
		}
		R methodProxy8(arglist ... args) {
			return methodProxy(8, args...);
		}
		R methodProxy9(arglist ... args) {
			return methodProxy(9, args...);
		}
		R methodProxy10(arglist ... args) {
			return methodProxy(10, args...);
		}
		R methodProxy11(arglist ... args) {
			return methodProxy(11, args...);
		}
		R methodProxy12(arglist ... args) {
			return methodProxy(12, args...);
		}
		R methodProxy13(arglist ... args) {
			return methodProxy(13, args...);
		}
		R methodProxy14(arglist ... args) {
			return methodProxy(14, args...);
		}
		R methodProxy15(arglist ... args) {
			return methodProxy(15, args...);
		}
		R methodProxy16(arglist ... args) {
			return methodProxy(16, args...);
		}
		R methodProxy17(arglist ... args) {
			return methodProxy(17, args...);
		}
		R methodProxy18(arglist ... args) {
			return methodProxy(18, args...);
		}
		R methodProxy19(arglist ... args) {
			return methodProxy(19, args...);
		}
		R methodProxy20(arglist ... args) {
			return methodProxy(20, args...);
		}
		R methodProxy21(arglist ... args) {
			return methodProxy(21, args...);
		}
		R methodProxy22(arglist ... args) {
			return methodProxy(22, args...);
		}
		R methodProxy23(arglist ... args) {
			return methodProxy(23, args...);
		}
		R methodProxy24(arglist ... args) {
			return methodProxy(24, args...);
		}
		R methodProxy25(arglist ... args) {
			return methodProxy(25, args...);
		}
		R methodProxy26(arglist ... args) {
			return methodProxy(26, args...);
		}
		R methodProxy27(arglist ... args) {
			return methodProxy(27, args...);
		}
		R methodProxy28(arglist ... args) {
			return methodProxy(28, args...);
		}
		R methodProxy29(arglist ... args) {
			return methodProxy(29, args...);
		}
		R methodProxy30(arglist ... args) {
			return methodProxy(30, args...);
		}
		R methodProxy31(arglist ... args) {
			return methodProxy(31, args...);
		}
		R methodProxy32(arglist ... args) {
			return methodProxy(32, args...);
		}
		R methodProxy33(arglist ... args) {
			return methodProxy(33, args...);
		}
		R methodProxy34(arglist ... args) {
			return methodProxy(34, args...);
		}
		R methodProxy35(arglist ... args) {
			return methodProxy(35, args...);
		}
		R methodProxy36(arglist ... args) {
			return methodProxy(36, args...);
		}
		R methodProxy37(arglist ... args) {
			return methodProxy(37, args...);
		}
		R methodProxy38(arglist ... args) {
			return methodProxy(38, args...);
		}
		R methodProxy39(arglist ... args) {
			return methodProxy(39, args...);
		}
		R methodProxy40(arglist ... args) {
			return methodProxy(40, args...);
		}
		R methodProxy41(arglist ... args) {
			return methodProxy(41, args...);
		}
		R methodProxy42(arglist ... args) {
			return methodProxy(42, args...);
		}
		R methodProxy43(arglist ... args) {
			return methodProxy(43, args...);
		}
		R methodProxy44(arglist ... args) {
			return methodProxy(44, args...);
		}
		R methodProxy45(arglist ... args) {
			return methodProxy(45, args...);
		}
		R methodProxy46(arglist ... args) {
			return methodProxy(46, args...);
		}
		R methodProxy47(arglist ... args) {
			return methodProxy(47, args...);
		}
		R methodProxy48(arglist ... args) {
			return methodProxy(48, args...);
		}
		R methodProxy49(arglist ... args) {
			return methodProxy(49, args...);
		}

		struct VirtualMethodProxy: public MethodProxy<R, arglist...> {

			VirtualMethodProxy(unsigned int offset, R(MethodProxyCreator::*vMethod)(arglist...)):
			_vMethod(vMethod), _offset(offset) {
			}

			unsigned int getOffset() override {
				return _offset;
			}

			void * getProxy() override {
				return union_cast<void *>(_vMethod);
			}

		private:
			R(MethodProxyCreator::*_vMethod)(arglist...);
			unsigned int _offset;
		};

	public:

		MethodProxy<R,arglist...>* createMethodProxy(unsigned int offset) {
			MethodProxy<R,arglist...>* proxy = nullptr;
			switch (offset) {
				case 0:
				proxy = new VirtualMethodProxy(0, &MethodProxyCreator::methodProxy0);
				break;
				case 1:
				proxy = new VirtualMethodProxy(1, &MethodProxyCreator::methodProxy1);
				break;
				case 2:
				proxy = new VirtualMethodProxy(2, &MethodProxyCreator::methodProxy2);
				break;
				case 3:
				proxy = new VirtualMethodProxy(3, &MethodProxyCreator::methodProxy3);
				break;
				case 4:
				proxy = new VirtualMethodProxy(4, &MethodProxyCreator::methodProxy4);
				break;
				case 5:
				proxy = new VirtualMethodProxy(5, &MethodProxyCreator::methodProxy5);
				break;
				case 6:
				proxy = new VirtualMethodProxy(6, &MethodProxyCreator::methodProxy6);
				break;
				case 7:
				proxy = new VirtualMethodProxy(7, &MethodProxyCreator::methodProxy7);
				break;
				case 8:
				proxy = new VirtualMethodProxy(8, &MethodProxyCreator::methodProxy8);
				break;
				case 9:
				proxy = new VirtualMethodProxy(9, &MethodProxyCreator::methodProxy9);
				break;
				case 10:
				proxy = new VirtualMethodProxy(10, &MethodProxyCreator::methodProxy10);
				break;
				case 11:
				proxy = new VirtualMethodProxy(11, &MethodProxyCreator::methodProxy11);
				break;
				case 12:
				proxy = new VirtualMethodProxy(12, &MethodProxyCreator::methodProxy12);
				break;
				case 13:
				proxy = new VirtualMethodProxy(13, &MethodProxyCreator::methodProxy13);
				break;
				case 14:
				proxy = new VirtualMethodProxy(14, &MethodProxyCreator::methodProxy14);
				break;
				case 15:
				proxy = new VirtualMethodProxy(15, &MethodProxyCreator::methodProxy15);
				break;
				case 16:
				proxy = new VirtualMethodProxy(16, &MethodProxyCreator::methodProxy16);
				break;
				case 17:
				proxy = new VirtualMethodProxy(17, &MethodProxyCreator::methodProxy17);
				break;
				case 18:
				proxy = new VirtualMethodProxy(18, &MethodProxyCreator::methodProxy18);
				break;
				case 19:
				proxy = new VirtualMethodProxy(19, &MethodProxyCreator::methodProxy19);
				break;
				case 20:
				proxy = new VirtualMethodProxy(20, &MethodProxyCreator::methodProxy20);
				break;
				case 21:
				proxy = new VirtualMethodProxy(21, &MethodProxyCreator::methodProxy21);
				break;
				case 22:
				proxy = new VirtualMethodProxy(22, &MethodProxyCreator::methodProxy22);
				break;
				case 23:
				proxy = new VirtualMethodProxy(23, &MethodProxyCreator::methodProxy23);
				break;
				case 24:
				proxy = new VirtualMethodProxy(24, &MethodProxyCreator::methodProxy24);
				break;
				case 25:
				proxy = new VirtualMethodProxy(25, &MethodProxyCreator::methodProxy25);
				break;
				case 26:
				proxy = new VirtualMethodProxy(26, &MethodProxyCreator::methodProxy26);
				break;
				case 27:
				proxy = new VirtualMethodProxy(27, &MethodProxyCreator::methodProxy27);
				break;
				case 28:
				proxy = new VirtualMethodProxy(28, &MethodProxyCreator::methodProxy28);
				break;
				case 29:
				proxy = new VirtualMethodProxy(29, &MethodProxyCreator::methodProxy29);
				break;
				case 30:
				proxy = new VirtualMethodProxy(30, &MethodProxyCreator::methodProxy30);
				break;
				case 31:
				proxy = new VirtualMethodProxy(31, &MethodProxyCreator::methodProxy31);
				break;
				case 32:
				proxy = new VirtualMethodProxy(32, &MethodProxyCreator::methodProxy32);
				break;
				case 33:
				proxy = new VirtualMethodProxy(33, &MethodProxyCreator::methodProxy33);
				break;
				case 34:
				proxy = new VirtualMethodProxy(34, &MethodProxyCreator::methodProxy34);
				break;
				case 35:
				proxy = new VirtualMethodProxy(35, &MethodProxyCreator::methodProxy35);
				break;
				case 36:
				proxy = new VirtualMethodProxy(36, &MethodProxyCreator::methodProxy36);
				break;
				case 37:
				proxy = new VirtualMethodProxy(37, &MethodProxyCreator::methodProxy37);
				break;
				case 38:
				proxy = new VirtualMethodProxy(38, &MethodProxyCreator::methodProxy38);
				break;
				case 39:
				proxy = new VirtualMethodProxy(39, &MethodProxyCreator::methodProxy39);
				break;
				case 40:
				proxy = new VirtualMethodProxy(40, &MethodProxyCreator::methodProxy40);
				break;
				case 41:
				proxy = new VirtualMethodProxy(41, &MethodProxyCreator::methodProxy41);
				break;
				case 42:
				proxy = new VirtualMethodProxy(42, &MethodProxyCreator::methodProxy42);
				break;
				case 43:
				proxy = new VirtualMethodProxy(43, &MethodProxyCreator::methodProxy43);
				break;
				case 44:
				proxy = new VirtualMethodProxy(44, &MethodProxyCreator::methodProxy44);
				break;
				case 45:
				proxy = new VirtualMethodProxy(45, &MethodProxyCreator::methodProxy45);
				break;
				case 46:
				proxy = new VirtualMethodProxy(46, &MethodProxyCreator::methodProxy46);
				break;
				case 47:
				proxy = new VirtualMethodProxy(47, &MethodProxyCreator::methodProxy47);
				break;
				case 48:
				proxy = new VirtualMethodProxy(48, &MethodProxyCreator::methodProxy48);
				break;
				case 49:
				proxy = new VirtualMethodProxy(49, &MethodProxyCreator::methodProxy49);
				break;
			}
			return proxy;
		}

		std::shared_ptr<MethodProxy<R, arglist...>> createMethodProxy(R (C::*vMethod)(arglist...)) {
			auto offset = getOffset(vMethod);
			MethodProxy<R, arglist...>* rv = createMethodProxy(offset);
			return std::shared_ptr<MethodProxy<R, arglist...>> {rv};
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
	typename VirtualTable<C, baseclasses...>::Handle originalVT; // avoid delete!! this is the original!
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
	void bind(std::shared_ptr<MethodProxy<R, arglist...>> methodProxy,
			std::shared_ptr<MethodInvocationHandler<R, arglist...>> invocationHandler) {
		auto offset = methodProxy->getOffset();
		getFake().setMethod(offset, methodProxy->getProxy());
		methodMocks[offset] = invocationHandler;
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
};
}
#endif // DynamicProxy_h__
