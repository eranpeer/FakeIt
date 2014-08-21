/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef VirtualTable_h__
#define VirtualTable_h__

#include "mockutils/gcc/is_simple_inheritance_layout.hpp"
#include "mockutils/VTUtils.hpp"

namespace fakeit {

template<class C, class ... baseclasses>
struct VirtualTable {

	static_assert(is_simple_inheritance_layout<C>::value, "Can't mock a type with multiple inheritance");

	class Handle {

		friend struct VirtualTable<C, baseclasses...> ;
		void** firstMethod;
		Handle(void** firstMethod) :
				firstMethod(firstMethod) {
		}

	public:

		VirtualTable<C, baseclasses...>& restore() {
			VirtualTable<C, baseclasses...>* vt = (VirtualTable<C, baseclasses...>*) this;
			return *vt;
		}
	};

	static VirtualTable<C, baseclasses...>& nullVTable() {
		static VirtualTable<C, baseclasses...> instance;
		return instance;
	}

	static VirtualTable<C, baseclasses...>& getVTable(C& instance) {
		fakeit::VirtualTable<C, baseclasses...>* vt = (fakeit::VirtualTable<C, baseclasses...>*) (&instance);
		return *vt;
	}

	void copyFrom(VirtualTable<C, baseclasses...>& from) {
		int size = VTUtils::getVTSize<C>();
		firstMethod[-1] = from.firstMethod[-1]; // copy type_info
		for (size_t i = 0; i < size; ++i) {
			firstMethod[i] = from.getMethod(i);
		}
	}

	VirtualTable() :
			VirtualTable(buildVTArray()) {
	}

	void dispose() {
		firstMethod--;
		firstMethod--;
		firstMethod--;
		delete[] firstMethod;
	}

	void setMethod(unsigned int index, void *method) {
		firstMethod[index] = method;
	}

	void * getMethod(unsigned int index) const {
		return firstMethod[index];
	}

	unsigned int getSize() {
		return VTUtils::getVTSize<C>();
	}

	void initAll(void* value) {
		unsigned int size = getSize();
		for (unsigned int i = 0; i < size; i++) {
			setMethod(i, value);
		}
	}

	const std::type_info* getTypeId() {
		return (const std::type_info*) (firstMethod[-1]);
	}

	void* getCookie() {
		return firstMethod[-3];
	}

	void setCookie(void * value) {
		firstMethod[-3] = value;
	}

	Handle createHandle() {
		Handle h(firstMethod);
		return h;
	}

private:

	void** firstMethod;

	static void ** buildVTArray() {
		int size = VTUtils::getVTSize<C>();
		auto array = new void*[size + 3] { };
		array[2] = (void*) &typeid(C);
		array++; // skip cookie
		array++; // skip top_offset
		array++; // skip type_info ptr
		return array;
	}

	VirtualTable(void** firstMethod) :
			firstMethod(firstMethod) {
	}

};
}
#endif // VirtualTable_h__
