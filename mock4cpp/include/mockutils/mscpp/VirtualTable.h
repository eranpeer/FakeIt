/*
 * VirtualTable.h
 *
 *  Created on: Feb 2, 2014
 *      Author: eran
 */

#ifndef VIRTUALTABLE_H_
#define VIRTUALTABLE_H_

typedef unsigned long DWORD;
struct TypeDescriptor
{
    DWORD ptrToVTable;
    DWORD spare;
    char name[8];
};

struct PMD
{
    int mdisp;  //member displacement
    int pdisp;  //vbtable displacement
    int vdisp;  //displacement inside vbtable
};
struct RTTIBaseClassDescriptor
{
    struct TypeDescriptor* pTypeDescriptor; //type descriptor of the class
    DWORD numContainedBases; //number of nested classes following in the Base Class Array
    struct PMD where;        //pointer-to-member displacement info
    DWORD attributes;        //flags, usually 0
};

struct RTTIClassHierarchyDescriptor
{
    DWORD signature;      //always zero?
    DWORD attributes;     //bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
    DWORD numBaseClasses; //number of classes in pBaseClassArray
    struct RTTIBaseClassArray* pBaseClassArray;
};

struct RTTICompleteObjectLocator
{
    DWORD signature; //always zero ?
    DWORD offset;    //offset of this vtable in the complete class
    DWORD cdOffset;  //constructor displacement offset
    struct TypeDescriptor* pTypeDescriptor; //TypeDescriptor of the complete class
    struct RTTIClassHierarchyDescriptor* pClassDescriptor; //describes inheritance hierarchy
};


template <int SIZE, class C>
struct VirtualTable {

	VirtualTable(){
		auto array = new void*[SIZE + 1];
		for (unsigned int i = 0; i < SIZE + 1; i++){
			array[i] = 0;
		}
		RTTICompleteObjectLocator * objectLocator = new RTTICompleteObjectLocator();
		objectLocator->signature = 0;
		objectLocator->cdOffset = 0;
		objectLocator->pClassDescriptor = nullptr;
		objectLocator->pClassDescriptor = nullptr;

		array[0] = objectLocator; // initialize type_info pointer
		firstMethod = array;
		firstMethod++; // skip objectLocator
	}

	~VirtualTable() {
		firstMethod--;
		RTTICompleteObjectLocator * objectLocator = (RTTICompleteObjectLocator *)firstMethod[0];
		delete objectLocator;
		delete [] firstMethod;
	}

	void setMethod(unsigned int index, void *method)
	{
		if (index >= SIZE){
			throw "error";
		}
		firstMethod[index] = method;
	}

	void * getMethod(unsigned int index){
		return firstMethod[index];
	}

	unsigned int getSize(){
		return SIZE;
	}

private:
	void** firstMethod;
};


#endif /* VIRTUALTABLE_H_ */
