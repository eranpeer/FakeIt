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
#include "mockutils/VirtualOffestSelector.hpp"

namespace fakeit {

static VirtualOffsetSelector offsetSelctor;

class VTUtils {
public:
	template<typename C, typename R, typename ... arglist>
	static unsigned int getOffset(R (C::*vMethod)(arglist...)) {
		auto sMethod = reinterpret_cast<unsigned int (VirtualOffsetSelector::*)()>(vMethod);
		auto selectMethod = std::bind(sMethod, &offsetSelctor);
		unsigned int offset = (unsigned int)selectMethod();
		return offset;
	}


	// Return the size of vTable starting at the index of the first method.
	// In a simple inheritance layout the size is the number of virtual methods in the table.
	template<typename C>
	static unsigned int getVTSize() {
		struct Tmp:public C {
			virtual void endOfVt(){}
		};
		unsigned int vtSize = getOffset(&Tmp::endOfVt);
		return vtSize;
	}

};


}


#endif /* VTUTILS_HPP_ */
