/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef Mock_h__
#define Mock_h__

#include <type_traits>
#include <unordered_set>

#include <memory>

#include "mockutils/DynamicProxy.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/DomainObjects.hpp"

namespace fakeit {

using namespace fakeit;

template<typename C, typename ... baseclasses>
class Mock: private MockObject<C>, public virtual ActualInvocationsSource {
private:
	DynamicProxy<C, baseclasses...> proxy;
	C* instance;bool isSpy;

	void unmocked() {
		throw UnexpectedMethodCallException { };
	}

	static C* createFakeInstance() {
		FakeObject<C, baseclasses...>* fake = new FakeObject<C, baseclasses...>();
		fake->initializeDataMembersArea();
		void* unmockedMethodStubPtr = union_cast<void*>(&Mock<C, baseclasses...>::unmocked);
		fake->getVirtualTable().initAll(unmockedMethodStubPtr);
		return (C*) fake;
	}

	template<typename R, typename ... arglist>
	MethodMock<C, R, arglist...>& stubMethodIfNotStubbed(DynamicProxy<C, baseclasses...> &proxy, R (C::*vMethod)(arglist...)) {
		if (!proxy.isStubbed(vMethod)) {
			auto methodMock = std::shared_ptr<MethodInvocationHandler<R, arglist...>> { new MethodMock<C, R, arglist...>(*this, vMethod) };
			proxy.stubMethod(vMethod, methodMock);
		}
		Destructable * d = proxy.getMethodMock(vMethod);
		MethodMock<C, R, arglist...> * methodMock = dynamic_cast<MethodMock<C, R, arglist...> *>(d);
		return *methodMock;
	}

	template<typename R, typename ... arglist>
	void * getOriginalMethod(R (C::*vMethod)(arglist...)) {
		auto vt = proxy.getOriginalVT();
		auto offset = VTUtils::getOffset(vMethod);
		void * origMethodPtr = vt.getMethod(offset);
		return origMethodPtr;
	}

	template<typename R, typename ... arglist>
	class MethodStubbingContextImpl: public MethodStubbingContext<C, R, arglist...> {
		Mock<C, baseclasses...>& mock;
		typename MethodStubbingContext<C, R, arglist...>::MethodType vMethod;
	public:

		MethodStubbingContextImpl(Mock<C, baseclasses...>& mock, R (C::*vMethod)(arglist...)) :
				mock(mock), vMethod(vMethod) {
		}
		virtual MethodMock<C, R, arglist...>& getMethodMock() override {
			return mock.stubMethodIfNotStubbed(mock.proxy, vMethod);
		}

		virtual void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
			mock.getActualInvocations(into);
		}

		virtual typename MethodStubbingContext<C, R, arglist...>::MethodType getMethod() override {
			return vMethod;
		}

		virtual typename MethodStubbingContext<C, R, arglist...>::MethodType getOriginalMethod() override {
			void * mPtr = mock.getOriginalMethod(vMethod);
			return union_cast<typename MethodStubbingContext<C, R, arglist...>::MethodType>(mPtr);
		}

		virtual MockObject<C>& getMock() override {
			return mock;
		}

	};

	template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> StubImpl(R (C::*vMethod)(arglist...)) {
		return FunctionStubbingRoot<C, R, arglist...>(
				std::shared_ptr<MethodStubbingContext<C, R, arglist...>>(new MethodStubbingContextImpl<R, arglist...>(*this, vMethod)));
	}

	template<typename R, typename ... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, R, arglist...> StubImpl(R (C::*vMethod)(arglist...)) {
		return ProcedureStubbingRoot<C, R, arglist...>(
				std::shared_ptr<MethodStubbingContext<C, R, arglist...>>(new MethodStubbingContextImpl<R, arglist...>(*this, vMethod)));
	}

	void Stub() {
	}

	template<class DATA_TYPE, typename ... arglist>
	DataMemberStubbingRoot<C, DATA_TYPE> stubDataMember(DATA_TYPE C::*member, const arglist&... ctorargs) {
		proxy.stubDataMember(member, ctorargs...);
		return DataMemberStubbingRoot<C, DATA_TYPE>();
	}

public:

	static_assert(std::is_polymorphic<C>::value, "Can only mock a polymorphic type");

	Mock() :
			Mock<C, baseclasses...>(*(createFakeInstance())) {
		isSpy = false;
	}

	Mock(C &obj) :
			proxy { obj }, instance(&obj), isSpy(true) {
	}

	/**
	 * Return all actual invocations of this mock.
	 */
	void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
		std::vector<ActualInvocationsSource*> vec;
		proxy.getMethodMocks(vec);
		for (ActualInvocationsSource * s : vec) {
			s->getActualInvocations(into);
		}
	}

	virtual ~Mock() {
		if (isSpy)
			return;
		FakeObject<C, baseclasses...>* fake = (FakeObject<C, baseclasses...>*) instance;
		delete fake;
	}

	virtual C& get() override {
		return proxy.get();
	}

	C& operator()() {
		return proxy.get();
	}

	void Reset() {
		proxy.Reset();
		if (!isSpy) {
			FakeObject<C, baseclasses...>* fake = (FakeObject<C, baseclasses...>*) instance;
			fake->initializeDataMembersArea();
		}
	}

	template<class DATA_TYPE, typename ... arglist,
			class = typename std::enable_if<std::is_member_object_pointer<DATA_TYPE C::*>::value>::type>
	DataMemberStubbingRoot<C, DATA_TYPE> Stub(DATA_TYPE C::* member, const arglist&... ctorargs) {
		return stubDataMember(member, ctorargs...);
	}

	template<typename H, typename ... M, class = typename std::enable_if<std::is_member_function_pointer<H>::value>::type>
	void Stub(H head, M ... tail) {
		When(head);
		Stub(tail...);
	}

	template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R (C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R (C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template < typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		return StubImpl(vMethod);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

};

}
#endif // Mock_h__
