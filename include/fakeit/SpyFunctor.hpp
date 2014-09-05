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

	template<typename C, typename R, typename ... arglist>
	void spy(const ActionSequenceBuilder<C, R, arglist...>& root) {
		ActionSequenceBuilder<C, R, arglist...>& rootWithoutConst = const_cast<ActionSequenceBuilder<C, R, arglist...>&>(root);
		C& obj = rootWithoutConst.get();
		auto methodFromOriginalVT = rootWithoutConst.getOriginalMethod();
		rootWithoutConst.appendAction(new ReturnDelegateValue<C, R, arglist...>(obj, methodFromOriginalVT));
		rootWithoutConst.commit();
	}

	void operator()() {
	}

public:

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
		spy(head);
		this->operator()(tail...);
	}

};

}

#endif /* FakeFunctor_hpp_ */
