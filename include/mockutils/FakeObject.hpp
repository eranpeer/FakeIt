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

template<typename C, typename ... baseclasses>
class FakeObject {

	VirtualTable<C, baseclasses...> vtable;
	char instanceArea[sizeof(C) -sizeof(VirtualTable<C, baseclasses...>)];

	FakeObject(FakeObject const &) = delete;            // undefined
	FakeObject& operator=(FakeObject const &) = delete; // undefined
public:

	FakeObject() :
			vtable() {
		initializeDataMembersArea();
	}

	void initializeDataMembersArea() {
		for (int i = 0; i < (sizeof(C) -sizeof(VirtualTable<C, baseclasses...>)); i++) {
			instanceArea[i] = (char) 0;
		}
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
}


#endif /* FAKEOBJECT_HPP_ */
