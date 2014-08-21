/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef Mock_h__
#define Mock_h__

#include "fakeit/DomainObjects.hpp"
#include "fakeit/MockImpl.hpp"

namespace fakeit {

using namespace fakeit;

template<typename C, typename ... baseclasses>
class Mock : private MockObject<C>, public ActualInvocationsSource {
	MockImpl<C, baseclasses...> impl;
public:

	static_assert(std::is_polymorphic<C>::value, "Can only mock a polymorphic type");

	Mock() : impl() {
	}

	Mock(C &obj) :impl(obj) {
	}

	virtual C& get() override {
		return impl.get();
	}

	C& operator()() {
		return get();
	}

	void Reset() {
		impl.reset();
	}

	template<class DATA_TYPE, typename ... arglist,
			class = typename std::enable_if<std::is_member_object_pointer<DATA_TYPE C::*>::value>::type>
	DataMemberStubbingRoot<C, DATA_TYPE> Stub(DATA_TYPE C::* member, const arglist&... ctorargs) {
		return impl.stubDataMember(member, ctorargs...);
	}

	template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R (C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<R (C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	template < typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<R(C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
	FunctionStubbingRoot<C, R, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		return impl.stub(vMethod);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...) const) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...) volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...) const volatile) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	template <typename R, typename... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
	ProcedureStubbingRoot<C, void, arglist...> operator [](R(C::*vMethod)(arglist...)) {
		auto methodWithoutConstVolatile = reinterpret_cast<void(C::*)(arglist...)>(vMethod);
		return impl.stub(methodWithoutConstVolatile);
	}

	void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
		impl.getActualInvocations(into);
	}
};

}
#endif // Mock_h__
