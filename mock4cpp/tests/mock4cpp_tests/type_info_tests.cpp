#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <iostream>
#include <type_traits>

#include "tpunit++.hpp"
#include "mock4cpp.h"
#include "mockutils/Formatter.h"

struct TypeInfoTests: tpunit::TestFixture {

	TypeInfoTests() :
			tpunit::TestFixture(
			//
					TEST(TypeInfoTests::mock_should_use_same_typeid_as_moked_class), //
					TEST(TypeInfoTests::simple_inheritance_upcast) //
					//, TEST(TypeInfoTests::try_type_info)//
							)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
	};

	void mock_should_use_same_typeid_as_moked_class() {
		Mock<SomeInterface> mock;
		ASSERT_EQUAL(typeid(mock.get()),typeid(SomeInterface));
	}

	struct TopLeft {
		int topLeft;
		virtual int f()=0;
	};

	struct Left: public TopLeft {
		int left;
		virtual int f() override = 0;
	};

	struct A: public Left {
		int a;
		virtual int f() override = 0;
	};

	void simple_inheritance_upcast() {
		Mock<A> aMock;
		A& a= aMock.get();
		Stub(aMock[&A::f]);

		Left& left= a;
		TopLeft& topLeft= left;

		ASSERT_EQUAL(0, a.f());
		ASSERT_EQUAL(0, left.f());
		ASSERT_EQUAL(0, topLeft.f());
	}

	/*
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
		Aclass* aPtr=new Aclass;
		Dclass* dPtr = new Dclass;
		//Aclass* aPtr=new Bclass;
		Aclass* cPtr = new Cclass;
		int ** aVFTPtr = (int**)(aPtr);
		RTTICompleteObjectLocator  aObjectLocatorPtr =
			*((RTTICompleteObjectLocator *)(*((int*)aVFTPtr[0] - 1)));

		int ** cVFTPtr = (int**)(cPtr);
		RTTICompleteObjectLocator  cObjectLocatorPtr =
			*((RTTICompleteObjectLocator *)(*((int*)cVFTPtr[0] - 1)));

		int ** dVFTPtr = (int**)(dPtr);
		RTTICompleteObjectLocator  dObjectLocatorPtr =
			*((RTTICompleteObjectLocator *)(*((int*)dVFTPtr[0] - 1)));


		unsigned long l1 = (unsigned long)aVFTPtr;
		unsigned long l2 = (unsigned long)aPtr;

		std::string classname(aObjectLocatorPtr.pTypeDescriptor->name);
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

		RTTICompleteObjectLocator  tiObjectLocatorPtr =
			*((RTTICompleteObjectLocator *)(*((int*)tiVFTPtr[0] - 1)));
	}
	*/
//
} __TypeInfoTests;
