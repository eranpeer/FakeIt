#ifndef Mock_h__
#define Mock_h__

#include <type_traits>
#include <unordered_set>

#include <memory>

#include "mockutils/DynamicProxy.h"

#include "fakeit/StubbingImpl.h"
#include "fakeit/WhenFunctor.hpp"
#include "fakeit/FakeFunctor.hpp"
#include "fakeit/VerifyFunctor.hpp"
#include "fakeit/DomainObjects.h"

namespace fakeit {

using namespace fakeit;

template<typename C, typename ... baseclasses>
class Mock: private MockObject, public virtual ActualInvocationsSource {
private:
	DynamicProxy<C, baseclasses...> instance;

	template<typename R, typename ... arglist>
	MethodMock<C, R, arglist...>& stubMethodIfNotStubbed(DynamicProxy<C, baseclasses...> &instance, R (C::*vMethod)(arglist...)) {
		if (!instance.isStubbed(vMethod)) {
			auto methodMock = std::shared_ptr<MethodInvocationHandler<R, arglist...>> { new MethodMock<C, R, arglist...>(*this, vMethod) };
			instance.stubMethod(vMethod, methodMock);
		}
		Destructable * d = instance.getMethodMock(vMethod);
		MethodMock<C, R, arglist...> * methodMock = dynamic_cast<MethodMock<C, R, arglist...> *>(d);
		return *methodMock;
	}

	template<typename R, typename ... arglist>
	class StubbingContextImpl: public StubbingContext<C, R, arglist...> {
		Mock<C, baseclasses...>& mock;
		R (C::*vMethod)(arglist...);
	public:
		StubbingContextImpl(Mock<C, baseclasses...>& mock, R (C::*vMethod)(arglist...)) :
				mock(mock), vMethod(vMethod) {
		}
		virtual MethodMock<C, R, arglist...>& getMethodMock() override {
			return mock.stubMethodIfNotStubbed(mock.instance, vMethod);
		}

		virtual void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
			mock.getActualInvocations(into);
		}
	};

	template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> StubImpl(R (C::*vMethod)(arglist...)) {
		return FunctionStubbingRoot<C, R, arglist...>(
				std::shared_ptr<StubbingContext<C, R, arglist...>>(new StubbingContextImpl<R, arglist...>(*this, vMethod)));
	}

	template<typename R, typename ... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, R, arglist...> StubImpl(R (C::*vMethod)(arglist...)) {
		return ProcedureStubbingRoot<C, R, arglist...>(
				std::shared_ptr<StubbingContext<C, R, arglist...>>(new StubbingContextImpl<R, arglist...>(*this, vMethod)));
	}

	void Stub() {
	}

	template<class DATA_TYPE, typename ... arglist>
	DataMemberStubbingRoot<C, DATA_TYPE> stubDataMember(DATA_TYPE C::*member, const arglist&... ctorargs) {
		instance.stubDataMember(member, ctorargs...);
		return DataMemberStubbingRoot<C, DATA_TYPE>();
	}

public:

	static_assert(std::is_polymorphic<C>::value, "Can only mock a polymorphic type");

	Mock() :
			MockObject { }, instance { [] {throw UnexpectedMethodCallException {};} } {
	}

	/**
	 * Return all actual invocations of this mock.
	 */
	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
		std::vector<ActualInvocationsSource*> vec;
		instance.getMethodMocks(vec);
		for (ActualInvocationsSource * s : vec) {
			s->getActualInvocations(into);
		}
	}

	virtual ~Mock() = default;

	C& get() {
		return instance.get();
	}

	C& operator()() {
		return instance.get();
	}

	void Reset() {
		instance.Reset();
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
		auto methodWithoutConstVolatile = reinterpret_cast<R (C::*)(arglist...)>(vMethod);returnStubImpl(methodWithoutConstVolatile);
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
	ProcedureStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		return StubImpl(vMethod);
	}

};

}
#endif // Mock_h__
