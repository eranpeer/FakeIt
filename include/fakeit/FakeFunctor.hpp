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

namespace fakeit {

class FakeFunctor {
private:
	void operator()() {
	}
public:

	FakeFunctor() {
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

}static Fake;

}

#endif /* FakeFunctor_hpp_ */
