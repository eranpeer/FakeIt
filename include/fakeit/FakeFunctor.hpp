/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeFunctor_hpp_
#define FakeFunctor_hpp_

#include "fakeit/Stubbing.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/FakeIt.hpp"

namespace fakeit {

class FakeFunctor {
private:
	void operator()() {
	}

	template<typename C, typename R, typename ... arglist>
	void fake(MethodStubbingBase<C, R, arglist...>& root) {
		std::shared_ptr<Behavior<R, arglist...>> ptr { new ReturnDefaultValue<R, arglist...>() };
		root.AppendAction(ptr);
		root.apply();
	}

public:

	FakeFunctor() {
		FakeIt::initIfNeeded();
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const ProcedureStubbingRoot<C, R, arglist...>& root) {
		ProcedureStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureStubbingRoot<C, R, arglist...>&>(root);
		fake(rootWithoutConst);
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const FunctionStubbingRoot<C, R, arglist...>& root) {
		FunctionStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<FunctionStubbingRoot<C, R, arglist...>&>(root);
		fake(rootWithoutConst);
	}

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
		this->operator()(head);
		this->operator()(tail...);
	}

}static Fake;

}

#endif /* FakeFunctor_hpp_ */
