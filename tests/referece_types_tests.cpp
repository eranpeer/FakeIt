/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct ReferenceTypesTests: tpunit::TestFixture {

	class AbstractType {
	public:
		virtual void foo() = 0;
	};

	class ConcreteType: public AbstractType {
	public:
		int state;
		ConcreteType(int value = 10) :
				state(value) {
		}
		virtual void foo() override {
		}

		bool operator==(const ConcreteType& other) {
			return (other.state == this->state);
		}

	};

	struct ReferenceInterface {
		virtual int& returnIntByRef() = 0;
		virtual AbstractType& returnAbstractTypeByRef() = 0;
		virtual ConcreteType& returnConcreteTypeByRef() = 0;
		virtual const std::string& returnStringByConstRef() = 0;
		virtual std::string& returnStringByRef() = 0;
	};

	ReferenceTypesTests() :
			tpunit::TestFixture(
					//
					TEST(ReferenceTypesTests::implicitStubbingDefaultReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefaultReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingReturnCopyValuesForRef),
					TEST(ReferenceTypesTests::explicitStubbingReturnValuesCopyForRRef),
					TEST(ReferenceTypesTests::explicitStubbingDefaultReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnCopyValuesForRef_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnCopyValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues_by_assignment),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues)
					//
							) {
	}

	void implicitStubbingDefaultReturnValues() {
		Mock<ReferenceInterface> mock;
		Fake( //
				Method(mock,returnIntByRef), //
				Method(mock,returnAbstractTypeByRef), //
				Method(mock,returnConcreteTypeByRef) //
				);

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingDefaultReturnValues() {
		Mock<ReferenceInterface> mock;		//
		When(Method(mock,returnIntByRef)).Return(); //
		When(Method(mock,returnAbstractTypeByRef)).Return(); //
		When(Method(mock,returnConcreteTypeByRef)).Return(); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingReturnValues() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		When(Method(mock,returnIntByRef)).Return(num); //
		When(Method(mock,returnConcreteTypeByRef)).Return(c); //
		When(Method(mock,returnAbstractTypeByRef)).Return(a); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(1, i.returnIntByRef());

		// Concrete types types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingReturnCopyValuesForRef() {
		Mock<ReferenceInterface> mock;

		// add scope so we know we are copying
		{
			std::string a_string{"myString"};
			int num{ 1 };

			// explicit copy here
			When(Method(mock, returnStringByConstRef)).ReturnCopy(a_string);
			When(Method(mock, returnIntByRef)).ReturnCopy(num);

			// modify now so know whether or not is was copied
			a_string = "modified";
			num = 2;
		}

		ReferenceInterface& i = mock.get();

		// Fundamental types are initiated to 0.
		EXPECT_EQUAL("myString", i.returnStringByConstRef());
		EXPECT_EQUAL(1, i.returnIntByRef());
	}

	void explicitStubbingReturnValuesCopyForRRef() {
		Mock<ReferenceInterface> mock;

		{
			When(Method(mock, returnStringByConstRef)).Return(std::string{ "myConstRefString" });
			When(Method(mock, returnStringByRef)).Return(std::string{ "myRefString" });
			When(Method(mock, returnConcreteTypeByRef)).Return(ConcreteType(20));
			When(Method(mock, returnIntByRef)).Return(1);
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("myConstRefString", i.returnStringByConstRef());
		EXPECT_EQUAL("myRefString", i.returnStringByRef());
		EXPECT_EQUAL(ConcreteType(20), i.returnConcreteTypeByRef());
		EXPECT_EQUAL(1, i.returnIntByRef());
	}

	void explicitStubbingReturnCopyValuesForRef_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;

		{
			When(Method(mock, returnStringByConstRef)).AlwaysReturn(std::string{ "myConstRefString" });
			When(Method(mock, returnStringByRef)).AlwaysReturn(std::string{ "myRefString" });
			When(Method(mock, returnConcreteTypeByRef)).AlwaysReturn(ConcreteType(20));
			When(Method(mock, returnIntByRef)).AlwaysReturn(1);
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("myConstRefString", i.returnStringByConstRef());
		EXPECT_EQUAL("myRefString", i.returnStringByRef());
		EXPECT_EQUAL(ConcreteType(20), i.returnConcreteTypeByRef());
		EXPECT_EQUAL(1, i.returnIntByRef());
	}

	void explicitStubbingReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		When(Method(mock,returnIntByRef)).AlwaysReturn(num); //
		When(Method(mock,returnConcreteTypeByRef)).AlwaysReturn(c); //
		When(Method(mock,returnAbstractTypeByRef)).AlwaysReturn(a); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(1, i.returnIntByRef());

		// Concrete types types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}
	
	void explicitStubbingReturnCopyValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;

		// add scope so we know we are copying
		{
			std::string a_string{ "myString" };
			int num{ 1 };

			// explicit copy here
			When(Method(mock, returnStringByConstRef)).AlwaysReturnCopy(a_string);
			When(Method(mock, returnIntByRef)).AlwaysReturnCopy(num);

			// modify now so know whether or not is was copied
			a_string = "modified";
			num = 2;
		}

		ReferenceInterface& i = mock.get();

		// Fundamental types are initiated to 0.
		EXPECT_EQUAL("myString", i.returnStringByConstRef());
		EXPECT_EQUAL("myString", i.returnStringByConstRef());

		EXPECT_EQUAL(1, i.returnIntByRef());
		EXPECT_EQUAL(1, i.returnIntByRef());
	}

	void explicitStubbingReturnValues_by_assignment() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		Method(mock,returnIntByRef) = num;
		Method(mock,returnConcreteTypeByRef) = c;
		Method(mock,returnAbstractTypeByRef) = a;

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(1, i.returnIntByRef());

		// Concrete types types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingDefaultReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;
		When(Method(mock,returnIntByRef)).AlwaysReturn();
		When(Method(mock,returnAbstractTypeByRef)).AlwaysReturn(); //
		When(Method(mock,returnConcreteTypeByRef)).AlwaysReturn(); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
	}

} __ReferenceTypesTests;

