#ifndef VirtualTable_h__
#define VirtualTable_h__

#include "mockutils/gcc/is_simple_inheritance_layout.hpp"

namespace fakeit {
	template<int SIZE, class C, class... baseclasses>
struct VirtualTable {

	static_assert(is_simple_inheritance_layout<C>::value, "Can't mock a type with multiple inheritance");

	VirtualTable() {
		auto array = new void*[SIZE + 2]{};
		array[1] = (void*) &typeid(C); // initialize type_info pointer
		firstMethod = array;
		firstMethod++; // top_offset
		firstMethod++; // type_info ptr
	}

	~VirtualTable() {
		firstMethod--;
		firstMethod--;
		delete[] firstMethod;
	}

	void setMethod(unsigned int index, void *method) {
		if (index >= SIZE) {
			throw "error";
		}
		firstMethod[index] = method;
	}

	void * getMethod(unsigned int index) {
		return firstMethod[index];
	}

	unsigned int getSize() {
		return SIZE;
	}

private:
	void** firstMethod;
};
}
#endif // VirtualTable_h__
