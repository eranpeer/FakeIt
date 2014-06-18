/*
 * VTUtils.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jun 3, 2014
 */
#ifndef VTUTILS_HPP_
#define VTUTILS_HPP_

#include <functional>
#include <type_traits>
#include "mockutils/VirtualOffestSelector.hpp"

namespace fakeit {

static VirtualOffsetSelector offsetSelctor;

class VTUtils {
public:
	template<typename C, typename R, typename ... arglist>
	static unsigned int getOffset(R (C::*vMethod)(arglist...)) {
		auto sMethod = reinterpret_cast<unsigned int (VirtualOffsetSelector::*)()>(vMethod);
		unsigned int offset = (offsetSelctor.*sMethod)();
		return offset;
	}

	template<typename C>
	static unsigned int getVTSize() {
		struct Derrived:public C{
			virtual void endOfVt(){}
		};

		unsigned int vtSize = getOffset(&Derrived::endOfVt);
		return vtSize;
	}

};


}


#endif /* VTUTILS_HPP_ */
