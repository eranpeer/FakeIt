#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <iostream>
#include <type_traits>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "mockutils/Formatter.h"

struct GccTypeInfoTests: tpunit::TestFixture {

	GccTypeInfoTests() :
			tpunit::TestFixture( //
					TEST(GccTypeInfoTests::simple_inheritance_dynamic_down_cast), TEST(GccTypeInfoTests::mutiple_inheritance_upcast)) {
	}
//
	//Aclass* aPtr=new Bclass;
//		A* cPtr = new Cclass;
//		int ** aVFTPtr = (int**) (aPtr);
//		RTTICompleteObjectLocator aObjectLocatorPtr = *((RTTICompleteObjectLocator *) (*((int*) aVFTPtr[0] - 1)));
//
//		int ** cVFTPtr = (int**) (cPtr);
//		RTTICompleteObjectLocator cObjectLocatorPtr = *((RTTICompleteObjectLocator *) (*((int*) cVFTPtr[0] - 1)));
//
//		int ** dVFTPtr = (int**) (dPtr);
//		RTTICompleteObjectLocator dObjectLocatorPtr = *((RTTICompleteObjectLocator *) (*((int*) dVFTPtr[0] - 1)));
//
//		unsigned long l1 = (unsigned long) aVFTPtr;
//		unsigned long l2 = (unsigned long) aPtr;

//	class Dclass {
//	public:
//		int a;
//		virtual void setA(int tmp) {
//			a = tmp;
//			std::cout << a << std::endl;
//		}
//	};

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

	void simple_inheritance_dynamic_down_cast() {
		Mock<A> aMock;
		Stub(aMock[&A::f]);
		TopLeft& topLeft = aMock.get();

		Left& left = dynamic_cast<Left&>(topLeft);
		A& a = dynamic_cast<A&>(topLeft);

		ASSERT_EQUAL(0, a.f());
		ASSERT_EQUAL(0, left.f());
		ASSERT_EQUAL(0, topLeft.f());
	}

	struct TopRight {
		int topRight;
		virtual int f()=0;
	};

	struct Right: public TopRight {
		int right;
		virtual int f() override = 0;
	};

	struct B: public Left, public Right {
		int b;
		virtual int f() override = 0;
	};

	void mutiple_inheritance_upcast() {
		//Mock<B> bMock; // should not compile
	}
//
} __GccTypeInfoTests;
