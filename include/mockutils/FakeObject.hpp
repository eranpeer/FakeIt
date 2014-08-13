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

#ifdef _WIN32
#include "mockutils/mscpp/VirtualTable.hpp"
#else
#include "mockutils/gcc/VirtualTable.hpp"
#endif

namespace fakeit {
// silent MSC++ compiler warning: C4200: nonstandard extension used : zero-sized array in struct/union.
#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4200 )
#endif


template<typename C, typename ... baseclasses>
class FakeObject {

    VirtualTable<C, baseclasses...> vtable;

    static const size_t SIZE = sizeof(C) - sizeof( VirtualTable<C, baseclasses...> );
#pragma GCC diagnostic ignored "-Wpedantic"
    char instanceArea[ SIZE ? SIZE : 0 ];
#pragma GCC diagnostic pop

    FakeObject(FakeObject const &) = delete;            // undefined
    FakeObject& operator=(FakeObject const &) = delete; // undefined

public:

    FakeObject() : vtable(VirtualTable<C, baseclasses...>::nullVTable()) {
        initializeDataMembersArea();
    }

    void initializeDataMembersArea() {
        for ( int i = 0; i < SIZE; ++i ) instanceArea[i] = (char) 0;
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


#ifdef _WIN32
#pragma warning( pop )
#endif


}


#endif /* FAKEOBJECT_HPP_ */
