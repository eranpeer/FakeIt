/*
 * VirtualTable.h
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

template<int SIZE, class C, class... baseclasses>
struct VirtualTable {
	
	VirtualTable() {
		auto array = new void*[SIZE + 1];
		for (unsigned int i = 0; i < SIZE + 1; i++) {
			array[i] = 0;
		}
		RTTICompleteObjectLocator<C, baseclasses...> * objectLocator = new RTTICompleteObjectLocator<C, baseclasses...>(typeid(C));
		array[0] = objectLocator; // initialize RTTICompleteObjectLocator pointer
		firstMethod = array;
		firstMethod++; // skip objectLocator
	}

	~VirtualTable() {
		firstMethod--;
		RTTICompleteObjectLocator<C> * objectLocator = (RTTICompleteObjectLocator<C> *) firstMethod[0];
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
