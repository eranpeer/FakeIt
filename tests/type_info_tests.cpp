#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <iostream>
#include <type_traits>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "mockutils/Formatter.h"

struct TypeInfoTests : tpunit::TestFixture {

	TypeInfoTests() :
	tpunit::TestFixture(
	//
	TEST(TypeInfoTests::mock_should_use_same_typeid_as_moked_class), //
	TEST(TypeInfoTests::simple_inheritance_upcast), //
	TEST(TypeInfoTests::dynamic_cast_to_same_type__with_concrete_type),
	TEST(TypeInfoTests::dynamic_cast_to_same_type__with_abstract_type),
	TEST(TypeInfoTests::simple_inheritance_dynamic_down_cast) //
<<<<<<< HEAD
	//TEST(TypeInfoTests::try_type_info)//
=======
>>>>>>> dfa783b6a8fecadace3c6da353ca52570dcf80c1
	)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
	};

	void mock_should_use_same_typeid_as_moked_class() {
		Mock<SomeInterface> mock;
		ASSERT_EQUAL(typeid(mock.get()), typeid(SomeInterface));
	}

	struct Demo {
		int demo;
		virtual int f() { return 0; };
	};

	struct TopLeft {
		int topLeft;
		virtual int l() = 0;
	};

	struct Left : public TopLeft {
		int left;
		virtual int l() override = 0;
	};

	struct TopRight {
		int topRight;
		virtual int r() = 0;
	};

	struct Right : public TopRight {
		int right;
		virtual int r() override = 0;
	};

	struct A : public Left, public Right 
	{
		int a;
		virtual int l() override { return 0; };
		virtual int r() override { return 0; };
	};

	void simple_inheritance_upcast() {

		Mock<A> aMock;
		aMock.enableRtti<Right>();
		A& a = aMock.get();
		Stub(aMock[&A::l]);

		Left& left = a;
		TopLeft& topLeft = left;

		ASSERT_EQUAL(0, a.l());
		ASSERT_EQUAL(0, left.l());
		ASSERT_EQUAL(0, topLeft.l());
	}

	void simple_inheritance_dynamic_down_cast() {
		
		Mock<A> aMock;
		aMock.enableRtti<Left>();
		Stub(aMock[&A::l]);
		A& a = aMock.get();
		Left* left = &a;

		A* aPtr = dynamic_cast<A*>(left);

 		ASSERT_EQUAL(0, aPtr->l());
	}

	void dynamic_cast_to_same_type__with_concrete_type()
	{
		Mock<Demo> mockDemo;
		Demo* mockDemoPtr = &mockDemo.get();
		Demo* mockDemoPtr2 = dynamic_cast<Demo*>(mockDemoPtr);
		ASSERT_EQUAL(mockDemoPtr, mockDemoPtr2);
	}

	void dynamic_cast_to_same_type__with_abstract_type()
	{
		Mock<TopLeft> mock;
		TopLeft* ptr = &mock.get();
		TopLeft* ptr2 = dynamic_cast<TopLeft*>(ptr);
		ASSERT_EQUAL(ptr, ptr2);
	}

<<<<<<< HEAD
//	template <typename C>
//	std::string to_string(struct RTTICompleteObjectLocator<C>* pObjectLocator){
//		return to_string(pObjectLocator->pClassDescriptor);
//	}
//
//	template <typename C>
//	std::string to_string(struct RTTIClassHierarchyDescriptor<C>* pClassDescriptor){
//		std::string result;
//		result += "RTTIClassHierarchyDescriptor {\nnumBaseClasses:"+ std::to_string(pClassDescriptor->numBaseClasses);
//		for (unsigned long i = 0; i < pClassDescriptor->numBaseClasses; i++){
//			RTTIBaseClassDescriptor * baseClassDesc = pClassDescriptor->pBaseClassArray[i];
//			result += "\n";
//			result += "RTTIBaseClassDescriptor {numContainedBases:" + std::to_string(baseClassDesc->numContainedBases) + "}";
//		}
//		return result;
//	}

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

//	void try_type_info()
//	{
//		Demo * demo = new Demo();
//		Mock<Demo> mockDemo;
//		Demo* mockDemoPtr = &mockDemo.get();
//		Demo* mockDemoPtr2 = dynamic_cast<Demo*>(mockDemoPtr);
//
//		A* aPtr = new A;
//		Dclass* dPtr = new Dclass;
//		//Aclass* aPtr=new Bclass;
//		Aclass* cPtr = new Cclass;
//		int ** aVFTPtr = (int**)(aPtr);
//		RTTICompleteObjectLocator<A>  aObjectLocatorPtr =
//			*((RTTICompleteObjectLocator<A> *)(*((int*)aVFTPtr[0] - 1)));
//
//		std::string aStr = to_string(&aObjectLocatorPtr);
//
//		int ** cVFTPtr = (int**)(cPtr);
//		RTTICompleteObjectLocator<Cclass>  cObjectLocatorPtr =
//			*((RTTICompleteObjectLocator<Cclass> *)(*((int*)cVFTPtr[0] - 1)));
//
//		int ** dVFTPtr = (int**)(dPtr);
//		RTTICompleteObjectLocator<Dclass>  dObjectLocatorPtr =
//			*((RTTICompleteObjectLocator<Dclass> *)(*((int*)dVFTPtr[0] - 1)));
//
//
//		unsigned long l1 = (unsigned long)aVFTPtr;
//		unsigned long l2 = (unsigned long)aPtr;
//
//		std::string classname(aObjectLocatorPtr.pTypeDescriptor->name());
//		classname = classname.substr(4, classname.find("@@") - 4);
//		std::cout << classname << std::endl;
//		const type_info& ti_aPtr = typeid(aPtr);
//		const type_info& ti_Aclass = typeid(Aclass);
//		const type_info& ti_Bclass = typeid(Bclass);
//		const type_info& ti_Cclass = typeid(Cclass);
//		const type_info& ti_Cclass2 = typeid(Cclass);
//
//		const type_info* ti_Cclassptr = &ti_Cclass;
//		const type_info* ti_Cclass2ptr = &ti_Cclass2;
//
//		int ** tiVFTPtr = (int**)(&typeid(void));
//		int * i = (int*)tiVFTPtr[0];
//		int type_info_vft_ptr = (int)i;
//
//		RTTICompleteObjectLocator<void>  tiObjectLocatorPtr =
//			*((RTTICompleteObjectLocator<void> *)(*((int*)tiVFTPtr[0] - 1)));
//	}


=======
>>>>>>> dfa783b6a8fecadace3c6da353ca52570dcf80c1
} __TypeInfoTests;
