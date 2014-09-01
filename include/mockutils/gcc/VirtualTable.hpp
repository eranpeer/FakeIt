/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef VirtualTable_h__
#define VirtualTable_h__

#ifndef __clang__
#include "mockutils/gcc/is_simple_inheritance_layout.hpp"
#endif

#include "mockutils/VTUtils.hpp"

namespace fakeit {

template<class C, class ... baseclasses>
struct VirtualTable {

	#ifndef __clang__
		static_assert(is_simple_inheritance_layout<C>::value, "Can't mock a type with multiple inheritance");
	#endif

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
		unsigned int size = VTUtils::getVTSize<C>();
		firstMethod[-1] = from.firstMethod[-1]; // copy type_info
		for (size_t i = 0; i < size; ++i) {
			firstMethod[i] = from.getMethod(i);
		}
	}

	VirtualTable() :
			VirtualTable(buildVTArray()) {
	}

	void dispose() {
		firstMethod--; // type_info
		firstMethod--; // top_offset
		firstMethod--; // cookie 0
		firstMethod--; // cookie 1
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

	void* getCookie(int index) {
		return firstMethod[-3 - index];
	}

	void setCookie(int index, void * value) {
		firstMethod[-3 - index] = value;
	}

	Handle createHandle() {
		Handle h(firstMethod);
		return h;
	}

private:

	void** firstMethod;

	static void ** buildVTArray() {
		int size = VTUtils::getVTSize<C>();
		auto array = new void*[size + 4] { };
		array[3] = (void*) &typeid(C);
		array++; // skip cookie 1
		array++; // skip cookie 0
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
