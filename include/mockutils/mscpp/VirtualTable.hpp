/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef VIRTUALTABLE_H_
#define VIRTUALTABLE_H_

#include "mockutils/VTUtils.hpp"

namespace fakeit {

typedef unsigned long DWORD;

struct TypeDescriptor {
	TypeDescriptor() :
			ptrToVTable(0), spare(0) {
		// ptrToVTable should contain the pointer to the virtual table of the type type_info!!!
		int ** tiVFTPtr = (int**) (&typeid(void));
		int * i = (int*) tiVFTPtr[0];
		int type_info_vft_ptr = (int) i;
		ptrToVTable = type_info_vft_ptr;
	}

	DWORD ptrToVTable;
	DWORD spare;
	char name[8];
};

struct PMD {

	/************************************************************************/
	/* member displacement. 
	/* For a simple inheritance structure the member displacement is always 0.
	/* since since the first member is placed at 0.
	/* In the case of multiple inheritance, this value may have a positive value.
	/************************************************************************/
	int mdisp;
	
	int pdisp;  // vtable displacement
	int vdisp;  //displacement inside vtable

	PMD() :
			mdisp(0), pdisp(-1), vdisp(0) {
	}
};
struct RTTIBaseClassDescriptor {
	RTTIBaseClassDescriptor() :
			pTypeDescriptor(nullptr), numContainedBases(0), attributes(0) {
	}

	const std::type_info* pTypeDescriptor; //type descriptor of the class
	DWORD numContainedBases; //number of nested classes following in the Base Class Array
	struct PMD where;        //pointer-to-member displacement info
	DWORD attributes;        //flags, usually 0
};

template <typename C, typename... baseclasses>
struct RTTIClassHierarchyDescriptor {
	RTTIClassHierarchyDescriptor() :
	signature(0), attributes(0), numBaseClasses(0),pBaseClassArray(nullptr){
		pBaseClassArray = new RTTIBaseClassDescriptor*[1 + sizeof...(baseclasses)];
		addBaseClass<C, baseclasses...>();
	}

	~RTTIClassHierarchyDescriptor(){
		for (int i = 0; i < 1 + sizeof...(baseclasses); i++){
			RTTIBaseClassDescriptor * desc = pBaseClassArray[i];
			delete desc;
		}
		delete[] pBaseClassArray;
	}
	
	DWORD signature;      //always zero?
	DWORD attributes;     //bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
	DWORD numBaseClasses; //number of classes in pBaseClassArray
	RTTIBaseClassDescriptor** pBaseClassArray;
	
	template<typename BaseType>
	void addBaseClass(){
		static_assert(std::is_base_of<BaseType, C>::value, "C must be a derived class of BaseType");
		RTTIBaseClassDescriptor* desc = new RTTIBaseClassDescriptor();
		desc->pTypeDescriptor = &typeid(BaseType);
		pBaseClassArray[numBaseClasses] = desc;
		for (unsigned int i = 0; i < numBaseClasses; i++) {
			pBaseClassArray[i]->numContainedBases++;
		}
		numBaseClasses++;
	}

	template<typename head,typename B1, typename... tail>
	void addBaseClass(){
		static_assert(std::is_base_of<B1, head>::value, "invalid inheritance list");
		addBaseClass<head>();
		addBaseClass<B1, tail...>();
	}

};

template<typename C, typename... baseclasses>
struct RTTICompleteObjectLocator {
	RTTICompleteObjectLocator(const std::type_info& info) :
	signature(0), offset(0), cdOffset(0), pTypeDescriptor(&info), pClassDescriptor(new RTTIClassHierarchyDescriptor<C,baseclasses...>()) {
	}

	~RTTICompleteObjectLocator(){
		delete pClassDescriptor;
	}

	DWORD signature; //always zero ?
	DWORD offset;    //offset of this vtable in the complete class
	DWORD cdOffset;  //constructor displacement offset
	const std::type_info* pTypeDescriptor; //TypeDescriptor of the complete class
	struct RTTIClassHierarchyDescriptor<C, baseclasses...>* pClassDescriptor; //describes inheritance hierarchy
};

template<class C, class... baseclasses>
struct VirtualTable {

	class Handle {

		friend struct VirtualTable<C,baseclasses...>;

		void** firstMethod;
		Handle(void** firstMethod) :firstMethod(firstMethod){}

	public:

		VirtualTable<C, baseclasses...>& restore(){
			VirtualTable<C, baseclasses...>* vt = (VirtualTable<C, baseclasses...>*)this;
			return *vt;
		}
	};

	static VirtualTable<C, baseclasses...>& nullVTable(){
		static VirtualTable<C, baseclasses...> instance;
		return instance;
	}

	static VirtualTable<C, baseclasses...>& getVTable(C& instance) {
		fakeit::VirtualTable<C, baseclasses...>* vt = (fakeit::VirtualTable<C, baseclasses...>*)(&instance);
		return *vt;
	}

	void copyFrom(VirtualTable<C, baseclasses...>& from) {
		unsigned int size = VTUtils::getVTSize<C>();
		firstMethod[-1] = from.firstMethod[-1]; // copy object locator
		for (unsigned int i = 0; i < size; i++) {
			firstMethod[i] = from.getMethod(i);
		}
	}

	VirtualTable(): VirtualTable(buildVTArray()) {
	}

	void dispose() {
		firstMethod--; // skip objectLocator
		firstMethod--; // skip cookie 0
		firstMethod--; // skip cookie 1
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

	void initAll(void* value){
		auto size = getSize();
		for (unsigned int i = 0; i < size; i++) {
			setMethod(i, value);
		}
	}


	void* getCookie(int index){
		return firstMethod[-2 - index];
	}

	void setCookie(int index, void * value){
		firstMethod[-2 - index] = value;
	}

	Handle createHandle() {
		Handle h(firstMethod);
		return h;
	}

private:

	void** firstMethod;

	class SimpleType {
	};

	static_assert(sizeof(unsigned int (SimpleType::*)()) == sizeof(unsigned int (C::*)()), "Can't mock a type with multiple inheritance");

	static void ** buildVTArray(){
		int size = VTUtils::getVTSize<C>();
		auto array = new void*[size + 3] {};
		RTTICompleteObjectLocator<C, baseclasses...> * objectLocator = new RTTICompleteObjectLocator<C, baseclasses...>(typeid(C));
		array[2] = objectLocator; // initialize RTTICompleteObjectLocator pointer
		array++; // skip cookie 1
		array++; // skip cookie 0
		array++; // skip object locator
		return array;
	}

	VirtualTable(void** firstMethod) :firstMethod(firstMethod){
	}

};
}
#endif /* VIRTUALTABLE_H_ */
