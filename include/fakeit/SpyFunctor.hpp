/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef SpyFunctor_hpp_
#define SpyFunctor_hpp_

#include "fakeit/Stubbing.hpp"
#include "fakeit/StubbingImpl.hpp"

namespace fakeit {

class SpyFunctor {
private:
	void operator()() {
	}

	template<typename C, typename R, typename ... arglist>
	void spy(MethodStubbingBase<C, R, arglist...>& root) {
		std::shared_ptr<Behavior<R, arglist...>> ptr {
			new ReturnDelegateValue<C, R, arglist...>(root.getMockObject().get(),root.getTheMethod()) };
		root.AppendAction(ptr);
		root.apply();
	}

public:

	SpyFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const ProcedureStubbingRoot<C, R, arglist...>& root) {
		ProcedureStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureStubbingRoot<C, R, arglist...>&>(root);
		spy(rootWithoutConst);
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const FunctionStubbingRoot<C, R, arglist...>& root) {
		FunctionStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<FunctionStubbingRoot<C, R, arglist...>&>(root);
		spy(rootWithoutConst);
	}

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
		this->operator()(head);
		this->operator()(tail...);
	}


}static Spy;

}

#endif /* FakeFunctor_hpp_ */
