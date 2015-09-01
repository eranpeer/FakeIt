/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifdef _MSC_VER

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include "wrl\implements.h"
#include "TlHelp32.h"

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct MscTypeInfoTests : tpunit::TestFixture {
	MscTypeInfoTests() :
	tpunit::TestFixture(
		TEST(MscTypeInfoTests::try_type_info)//
	)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
	};

	template <typename C, typename... baseclasses>
	std::string to_string(struct RTTICompleteObjectLocator<C, baseclasses...>* pObjectLocator){
		return to_string(pObjectLocator->pClassDescriptor);
	}

	template <typename C, typename... baseclasses>
	std::string to_string(struct RTTIClassHierarchyDescriptor<C, baseclasses...>* pClassDescriptor){
		std::string result;
		result += "RTTIClassHierarchyDescriptor {\nnumBaseClasses:" + fakeit::to_string(pClassDescriptor->numBaseClasses);
		for (unsigned long i = 0; i < pClassDescriptor->numBaseClasses; i++){
			RTTIBaseClassDescriptor * baseClassDesc = pClassDescriptor->pBaseClassArray[i];
			result += "\n";
			result += "RTTIBaseClassDescriptor {numContainedBases:" + fakeit::to_string(baseClassDesc->numContainedBases) + "}";
		}
		return result;
	}

	class Dclass
	{
	public:
		int a;
		virtual void setA(int tmp)
		{
			a = tmp;
			std::cout << a << std::endl;
		}
	};

	class Aclass
	{
	public:
		int a;
		virtual void setA(int tmp)
		{
			a = tmp;
			std::cout << a << std::endl;
		}
	};

	class Bclass :public Aclass
	{
	public:
		virtual void setA(int tmp)
		{
			a = tmp + 10;
			std::cout << a << std::endl;
		}
	public:
		void print()
		{
			std::cout << a << std::endl;
		}
	};

	class Cclass :public Bclass
	{
	};

#ifdef _WIN64 
	/*
	 *  for how this works, see https://msdn.microsoft.com/en-us/library/windows/desktop/ms686849(v=vs.85).aspx
	 */
	DWORD_PTR findBaseAddress(DWORD_PTR address) {
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32;

		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
		if (hModuleSnap == INVALID_HANDLE_VALUE) {
			return 0;
		}
		me32.dwSize = sizeof(MODULEENTRY32);
		if (!Module32First(hModuleSnap, &me32)) {
			CloseHandle(hModuleSnap);
			return 0;
		}
			
		DWORD_PTR retval = 0;
		do
		{
			auto start = me32.modBaseAddr;
			auto end = start + me32.modBaseSize;
			if (start <= (BYTE *)address && (BYTE *)address <= end) {
				retval = (DWORD_PTR)start;
				break;
			}
		} while (Module32Next(hModuleSnap, &me32));
		CloseHandle(hModuleSnap);
		return retval;
	}
#endif

	void try_type_info()
	{
#ifdef _WIN64
		Aclass* aPtr = new Bclass;
		uint64_t ** aVFTPtr = (uint64_t**)(aPtr);
		RTTICompleteObjectLocator<Aclass>*  aObjectLocatorPtr =
			((RTTICompleteObjectLocator<Aclass> *)(*((uint64_t*)aVFTPtr[0] - 1)));

		//Find pointer to std::type_info from the BaseAddress of this module and the typeDescriptorOffset found in *aVFTPtr[-1]
		DWORD_PTR baseAddress = findBaseAddress(reinterpret_cast<DWORD_PTR>(aObjectLocatorPtr));
		auto type_info_ptr = reinterpret_cast<const std::type_info *>((uint64_t)baseAddress + aObjectLocatorPtr->typeDescriptorOffset);
		
		//Get and print class name
		std::string classname{ type_info_ptr->name() };
		std::cout << classname << std::endl;

		//Assert that type_info read from typeid() is the same as the one read through type_info_ptr we found previously.
		auto &type_info_of_Bclass = typeid(Bclass);
		ASSERT_EQUAL(std::string{ type_info_ptr->name() }, std::string{ type_info_of_Bclass.name() });

#else
		Dclass* dPtr = new Dclass;
		Aclass* aPtr=new Bclass;
		Aclass* cPtr = new Cclass;
		int ** aVFTPtr = (int**)(aPtr);
		RTTICompleteObjectLocator<Aclass>*  aObjectLocatorPtr =
			((RTTICompleteObjectLocator<Aclass> *)(*((int*)aVFTPtr[0] - 1)));

		//std::string aStr = to_string(aObjectLocatorPtr);

		int ** cVFTPtr = (int**)(cPtr);
		RTTICompleteObjectLocator<Cclass>*  cObjectLocatorPtr =
			((RTTICompleteObjectLocator<Cclass> *)(*((int*)cVFTPtr[0] - 1)));

		int ** dVFTPtr = (int**)(dPtr);
		RTTICompleteObjectLocator<Dclass>*  dObjectLocatorPtr =
			((RTTICompleteObjectLocator<Dclass> *)(*((int*)dVFTPtr[0] - 1)));

		unsigned long l1 = (unsigned long)aVFTPtr;
		unsigned long l2 = (unsigned long)aPtr;

		std::string classname(aObjectLocatorPtr->pTypeDescriptor->name());
		classname = classname.substr(4, classname.find("@@") - 4);
		std::cout << classname << std::endl;
		const type_info& ti_aPtr = typeid(aPtr);
		const type_info& ti_Aclass = typeid(Aclass);
		const type_info& ti_Bclass = typeid(Bclass);
		const type_info& ti_Cclass = typeid(Cclass);
		const type_info& ti_Cclass2 = typeid(Cclass);

		const type_info* ti_Cclassptr = &ti_Cclass;
		const type_info* ti_Cclass2ptr = &ti_Cclass2;

		int ** tiVFTPtr = (int**)(&typeid(void));
		int * i = (int*)tiVFTPtr[0];
		int type_info_vft_ptr = (int)i;

		RTTICompleteObjectLocator<void>*  tiObjectLocatorPtr =
			((RTTICompleteObjectLocator<void> *)(*((int*)tiVFTPtr[0] - 1)));
#endif
	}

} __MscTypeInfoTests;

#endif
