/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef SpyFunctor_hpp_
#define SpyFunctor_hpp_

#include "fakeit/StubbingProgress.hpp"
#include "fakeit/StubbingImpl.hpp"

namespace fakeit {

class SpyFunctor {
private:
	void operator()() {
	}

	template<typename C, typename R, typename ... arglist>
	void spy(ActionSequenceBuilder<C, R, arglist...>& builder) {
		C& obj = builder.get();
		auto methodFromOriginalVT = builder.getOriginalMethod();
		builder.appendAction(new ReturnDelegateValue<C, R, arglist...>(obj, methodFromOriginalVT));
		builder.commit();
	}

public:

	SpyFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const ProcedureSequenceBuilder<C, R, arglist...>& root) {
		ProcedureSequenceBuilder<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureSequenceBuilder<C, R, arglist...>&>(root);
		spy(rootWithoutConst);
	}

	template<typename C, typename R, typename ... arglist>
	void operator()(const FunctionSequenceBuilder<C, R, arglist...>& root) {
		FunctionSequenceBuilder<C, R, arglist...>& rootWithoutConst = const_cast<FunctionSequenceBuilder<C, R, arglist...>&>(root);
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
