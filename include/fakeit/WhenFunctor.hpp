/*
 * WhenFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: eran
 */

#ifndef WHENFUNCTOR_HPP_
#define WHENFUNCTOR_HPP_

#include "fakeit/StubbingImpl.h"
#include "fakeit/Stubbing.h"

namespace fakeit {

class WhenFunctor {
public:
	WhenFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	FirstProcedureStubbingProgress<R, arglist...>& operator()(const ProcedureStubbingRoot<C, R, arglist...>& stubbingProgress) {
		ProcedureStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureStubbingRoot<C, R, arglist...>&>(stubbingProgress);
		return dynamic_cast<FirstProcedureStubbingProgress<R, arglist...>&>(rootWithoutConst);
	}

	template<typename C, typename R, typename ... arglist>
	FirstFunctionStubbingProgress<R, arglist...>& operator()(const FunctionStubbingRoot<C, R, arglist...>& stubbingProgress) {
		FunctionStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<FunctionStubbingRoot<C, R, arglist...>&>(stubbingProgress);
		return dynamic_cast<FirstFunctionStubbingProgress<R, arglist...>&>(rootWithoutConst);
	}

}static When;

}

#endif /* WHENFUNCTOR_HPP_ */
