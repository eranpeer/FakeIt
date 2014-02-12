#ifndef VirtualTable_h__
#define VirtualTable_h__

#include <type_traits>
#include <tr2/type_traits>

template<typename... T1>
class has_one_base {};

template<typename T1, typename T2, typename ... types>
class has_one_base<std::tr2::__reflection_typelist<T1, T2,  types...>> : public std::false_type {
};

template<typename T1>
class has_one_base<std::tr2::__reflection_typelist<T1>> : public has_one_base<typename std::tr2::direct_bases<T1>::type> {
};

template<>
class has_one_base<std::tr2::__reflection_typelist<>> : public std::true_type {
};



template<typename T>
class is_simple_inheritance_layout : public has_one_base<typename std::tr2::direct_bases<T>::type> {
};


template<int SIZE, class C>
struct VirtualTable {

	static_assert(is_simple_inheritance_layout<C>::value, "Can't mock a type with multiple inheritance");

	VirtualTable() {

		auto array = new void*[SIZE + 2];
		for (unsigned int i = 0; i < SIZE + 2; i++) {
			array[i] = 0;
		}
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
#endif // VirtualTable_h__
