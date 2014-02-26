/*
 * VirtualTable.h
 *
 *  Created on: Feb 2, 2014
 *      Author: eran
 */

#ifndef VIRTUALTABLE_H_
#define VIRTUALTABLE_H_
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
	int mdisp;  //member displacement
	int pdisp;  //vbtable displacement
	int vdisp;  //displacement inside vbtable

	PMD() :
			mdisp(0), pdisp(0), vdisp(0) {
	}
};
struct RTTIBaseClassDescriptor {
	RTTIBaseClassDescriptor() :
			pTypeDescriptor(nullptr), numContainedBases(0), attributes(0) {
	}

	struct TypeDescriptor* pTypeDescriptor; //type descriptor of the class
	DWORD numContainedBases; //number of nested classes following in the Base Class Array
	struct PMD where;        //pointer-to-member displacement info
	DWORD attributes;        //flags, usually 0
};

struct RTTIClassHierarchyDescriptor {
	RTTIClassHierarchyDescriptor() :
			signature(0), attributes(0), numBaseClasses(0), pBaseClassArray(nullptr) {
	}

	DWORD signature;      //always zero?
	DWORD attributes;     //bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
	DWORD numBaseClasses; //number of classes in pBaseClassArray
	RTTIBaseClassDescriptor **pBaseClassArray;
};

struct RTTICompleteObjectLocator {
	RTTICompleteObjectLocator() :
			signature(0), offset(0), cdOffset(0), pTypeDescriptor(nullptr), pClassDescriptor(nullptr) {
	}
	DWORD signature; //always zero ?
	DWORD offset;    //offset of this vtable in the complete class
	DWORD cdOffset;  //constructor displacement offset
	struct TypeDescriptor* pTypeDescriptor; //TypeDescriptor of the complete class
	struct RTTIClassHierarchyDescriptor* pClassDescriptor; //describes inheritance hierarchy
};

template<int SIZE, class C>
struct VirtualTable {

	VirtualTable() {
		auto array = new void*[SIZE + 1];
		for (unsigned int i = 0; i < SIZE + 1; i++) {
			array[i] = 0;
		}
		RTTICompleteObjectLocator * objectLocator = new RTTICompleteObjectLocator();
		TypeDescriptor* typeDescriptorPtr = (TypeDescriptor*) &typeid(C);
		objectLocator->pTypeDescriptor = typeDescriptorPtr;

		//RTTIClassHierarchyDescriptor* classDescriptorPtr = new RTTIClassHierarchyDescriptor();
		//classDescriptorPtr->numBaseClasses = 1;
		//RTTIBaseClassDescriptor * baseClassDescriptor = new RTTIBaseClassDescriptor();
		//RTTIBaseClassDescriptor ** baseClassDescriptorArr = new RTTIBaseClassDescriptor*[1];
		//baseClassDescriptorArr[0] = baseClassDescriptor;
		//objectLocator->pClassDescriptor = classDescriptorPtr;

		array[0] = objectLocator; // initialize RTTICompleteObjectLocator pointer
		firstMethod = array;
		firstMethod++; // skip objectLocator
	}

	~VirtualTable() {
		firstMethod--;
		RTTICompleteObjectLocator * objectLocator = (RTTICompleteObjectLocator *) firstMethod[0];
		delete objectLocator;
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
#endif /* VIRTUALTABLE_H_ */
