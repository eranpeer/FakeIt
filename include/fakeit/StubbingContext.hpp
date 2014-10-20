/*
 * StubbingContext.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 20, 2014
 */
#ifndef STUBBINGCONTEXT_HPP_
#define STUBBINGCONTEXT_HPP_

#include "fakeit/Xaction.hpp"

namespace fakeit {

// For use in Fake & When phrases
template<typename R, typename ... arglist>
struct StubbingContext : public Xaction {
	virtual void appendAction(Action<R, arglist...>* action) = 0;
};
}

#endif /* STUBBINGCONTEXT_HPP_ */
