#ifndef Mock_h__
#define Mock_h__

#include <type_traits>
#include <memory>

#include "mockutils/DynamicProxy.h"
#include "mock4cpp/StubbingImpl.h"
#include "mock4cpp/MockRepository.h"

using namespace mock4cpp;

template<typename C>
class Mock: private MockBase, public virtual ActualInvocationsSource {
private:
	DynamicProxy<C> instance;

	template<typename R, typename ... arglist>
	MethodMock<R, arglist...>& stubMethodIfNotStubbed(DynamicProxy<C> &instance, R (C::*vMethod)(arglist...)) {
		if (!instance.isStubbed(vMethod)) {
			auto methodMock = std::shared_ptr<MethodInvocationHandler<R, arglist...>> { new MethodMock<R, arglist...>(*this) };
			instance.stubMethod(vMethod, methodMock);
		}
		Destructable * d = instance.getMethodMock(vMethod);
		MethodMock<R, arglist...> * methodMock = dynamic_cast<MethodMock<R, arglist...> *>(d);
		return *methodMock;
	}

	template<typename R, typename ... arglist>
	class StubbingContextImpl: public StubbingContext<R, arglist...> {
		Mock<C>& mock;
		R (C::*vMethod)(arglist...);
	public:
		StubbingContextImpl(Mock<C>& mock, R (C::*vMethod)(arglist...)) :
				mock(mock), vMethod(vMethod) {
		}
		virtual MethodMock<R, arglist...>& getMethodMock() override {
			return mock.stubMethodIfNotStubbed(mock.instance, vMethod);
		}
	};

	template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> StubImpl(R (C::*vMethod)(arglist...)) {
		return FunctionStubbingRoot<R, arglist...>(
				std::shared_ptr<StubbingContext<R, arglist...>>(new StubbingContextImpl<R, arglist...>(*this, vMethod)));
	}

	template<typename R, typename ... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> StubImpl(R (C::*vMethod)(arglist...)) {
		return ProcedureStubbingRoot<R, arglist...>(
				std::shared_ptr<StubbingContext<R, arglist...>>(new StubbingContextImpl<R, arglist...>(*this, vMethod)));
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
			MockBase { }, instance { [] {throw UnmockedMethodCallException {};} } {
	}

	/**
	 * Return all actual invocations of this mock.
	 */
	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
		std::vector<ActualInvocationsSource*> vec;
		instance.getMethodMocks(vec);
		for (ActualInvocationsSource * s : vec){
			s->getActualInvocations(into);
		}
	}

	~Mock() {
	}

	C& get() {
		return instance.get();
	}

	C& operator()() {
		return instance.get();
	}

	template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> When(R (C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R (C::*)(arglist...)>(vMethod);return StubImpl(methodWithoutConstVolatile);
	}

	template < typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...)) {
		return StubImpl(vMethod);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> When(R(C::*vMethod)(arglist...)) {
		return StubImpl(vMethod);
	}

	template <class DATA_TYPE, typename... arglist
	, class = typename std::enable_if<std::is_member_object_pointer<DATA_TYPE C::*>::value>::type
	>
	DataMemberStubbingRoot<C, DATA_TYPE> Stub(DATA_TYPE C::* member, const arglist&... ctorargs)
	{
		return stubDataMember(member, ctorargs...);
	}

	template <typename H, typename... M
	, class = typename std::enable_if<std::is_member_function_pointer<H>::value>::type
	>
	void Stub(H head, M... tail)
	{
		((MethodStubbingInternal&)When(head)).startStubbing();
		Stub(tail...);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template < typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		return StubImpl(vMethod);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return StubImpl(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<R, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		return StubImpl(vMethod);
	}

};

#endif // Mock_h__
