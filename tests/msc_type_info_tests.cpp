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

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"
#include "mockutils/to_string.hpp"

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

	void try_type_info()
	{
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
	}

} __MscTypeInfoTests;

#endif
