/*
 * StubFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: eran
 */

#ifndef STUBFUNCTOR_HPP_
#define STUBFUNCTOR_HPP_

#include "fakeit/StubbingImpl.h"
#include "fakeit/Stubbing.h"

namespace fakeit {

class StubFunctor {
private:
	void operator()() {
	}
public:
	StubFunctor() {
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

}static Stub;

}

#endif /* STUBFUNCTOR_HPP_ */
