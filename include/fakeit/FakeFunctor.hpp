/*
 * StubFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: eran
 */

#ifndef FakeFunctor_hpp_
#define FakeFunctor_hpp_

#include "fakeit/Stubbing.h"
#include "fakeit/StubbingImpl.h"

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
