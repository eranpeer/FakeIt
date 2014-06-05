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
public:

	SpyFunctor() {
	}

	template<typename H>
	void operator()(const H& head) {
		H& headWithoutConst = const_cast<H&>(head);
		headWithoutConst.apply();
	}

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
		H& headWithoutConst = const_cast<H&>(head);
		headWithoutConst.apply();
		this->operator()(tail...);
	}

}static Spy;

}

#endif /* FakeFunctor_hpp_ */
