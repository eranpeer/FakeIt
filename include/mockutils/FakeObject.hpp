/*
 * FakeObject.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jun 8, 2014
 */
#ifndef FAKEOBJECT_HPP_
#define FAKEOBJECT_HPP_

#include <new>

#ifdef _MSC_VER
#include "mockutils/mscpp/VirtualTable.hpp"
#else
#include "mockutils/gcc/VirtualTable.hpp"
#endif

namespace fakeit {
// silent GCC compiler warning: iso c++ forbids zero-size array [-Wpedantic]
#ifdef __GNUG__
  #ifndef __clang__
    #pragma GCC diagnostic ignored "-Wpedantic"
  #endif
#endif

// silent MSC++ compiler warning: C4200: nonstandard extension used : zero-sized array in struct/union.
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4200 )
#endif


template<typename C, typename ... baseclasses>
class FakeObject {

    VirtualTable<C, baseclasses...> vtable;

    static const size_t SIZE = sizeof(C) - sizeof( VirtualTable<C, baseclasses...> );
    char instanceArea[ SIZE ? SIZE : 0 ];

    FakeObject(FakeObject const &) = delete;            // undefined
    FakeObject& operator=(FakeObject const &) = delete; // undefined

public:

    FakeObject() : vtable(VirtualTable<C, baseclasses...>::nullVTable()) {
        initializeDataMembersArea();
    }

    void initializeDataMembersArea() {
        for ( size_t i = 0; i < SIZE; ++i ) instanceArea[i] = (char) 0;
    }

	void setMethod(unsigned int index, void *method) {
		vtable.setMethod(index, method);
	}

	VirtualTable<C, baseclasses...>& getVirtualTable() {
		return vtable;
	}

	void setVirtualTable(VirtualTable<C, baseclasses...>& t) {
		vtable = t;
	}
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#ifdef __GNUG__
  #ifndef __clang__
    #pragma GCC diagnostic pop
  #endif
#endif

}


#endif /* FAKEOBJECT_HPP_ */
