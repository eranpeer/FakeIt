/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeFunctor_hpp_
#define FakeFunctor_hpp_

#include "fakeit/StubbingProgress.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/FakeitContext.hpp"

namespace fakeit {

class FakeFunctor {
private:
	void operator()() {
	}

	template<typename C, typename R, typename ... arglist>
	void fake(ActionSequenceBuilder<C, R, arglist...>& root) {
		root.appendAction(new ReturnDefaultValue<R, arglist...>());
		root.commit();
	}

public:

	FakeFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const ProcedureSequenceBuilder<C, R, arglist...>& root) {
		ProcedureSequenceBuilder<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureSequenceBuilder<C, R, arglist...>&>(root);
		fake(rootWithoutConst);
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const FunctionSequenceBuilder<C, R, arglist...>& root) {
		FunctionSequenceBuilder<C, R, arglist...>& rootWithoutConst = const_cast<FunctionSequenceBuilder<C, R, arglist...>&>(root);
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
