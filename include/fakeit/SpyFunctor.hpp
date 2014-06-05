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
	}

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
	}

}static Spy;

}

#endif /* FakeFunctor_hpp_ */
