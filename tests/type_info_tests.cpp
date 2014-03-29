#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <iostream>
#include <type_traits>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.h"

using namespace fakeit;

struct TypeInfoTests : tpunit::TestFixture {

	TypeInfoTests() :
	tpunit::TestFixture(
	//
	TEST(TypeInfoTests::mock_should_use_same_typeid_as_moked_class), //
	TEST(TypeInfoTests::simple_inheritance_upcast), //
	TEST(TypeInfoTests::dynamic_cast_to_same_type__with_concrete_type),
	TEST(TypeInfoTests::dynamic_cast_to_same_type__with_abstract_type),
	TEST(TypeInfoTests::simple_inheritance_dynamic_down_cast) //
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

	struct ConcreteType {
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

	struct A : public Left
	//, public Right
	{
		int a;
		virtual int l() override { return 0; };
		//virtual int r() override { return 0; };
	};

	void simple_inheritance_upcast() {

		Mock<A> aMock;
		A& a = aMock.get();
		Fake(aMock[&A::l]);

		Left& left = a;
		TopLeft& topLeft = left;

		ASSERT_EQUAL(0, a.l());
		ASSERT_EQUAL(0, left.l());
		ASSERT_EQUAL(0, topLeft.l());
	}

	void simple_inheritance_dynamic_down_cast() {
		Mock<A, Left, TopLeft> aMock;
		Fake(aMock[&A::l]);
		A& a = aMock.get();
		Left* left = &a;
		TopLeft* topLeft = &a;

		A* aPtr = dynamic_cast<A*>(left);
 		ASSERT_EQUAL(0, aPtr->l());

 		aPtr = dynamic_cast<A*>(topLeft);
  		ASSERT_EQUAL(0, aPtr->l());
 
  		left = dynamic_cast<Left*>(topLeft);
  		ASSERT_EQUAL(0, left->l());
	}

	void dynamic_cast_to_same_type__with_concrete_type()
	{
		Mock<ConcreteType> mockDemo;
		ConcreteType* mockDemoPtr = &mockDemo.get();
		ConcreteType* mockDemoPtr2 = dynamic_cast<ConcreteType*>(mockDemoPtr);
		ASSERT_EQUAL(mockDemoPtr, mockDemoPtr2);
	}

	void dynamic_cast_to_same_type__with_abstract_type()
	{
		Mock<TopLeft> mock;
		TopLeft* ptr = &mock.get();
		TopLeft* ptr2 = dynamic_cast<TopLeft*>(ptr);
		ASSERT_EQUAL(ptr, ptr2);
	}

} __TypeInfoTests;
