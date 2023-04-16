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
		ConcreteType() :
				state(10) {
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
	};

	ReferenceTypesTests() :
			tpunit::TestFixture(
					//
					TEST(ReferenceTypesTests::implicitStubbingDefaultReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefualtReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefualtReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues_by_assignment),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues)
					//
							) {
	}

	void implicitStubbingDefaultReturnValues() {
		Mock<ReferenceInterface> mock;
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		Fake( //
				Method(mock,returnIntByRef), //
				Method(mock,returnAbstractTypeByRef), //
				Method(mock,returnConcreteTypeByRef) //
				);
#else
		Fake( //
				Method(mock,returnIntByRef), //
				Method(mock,returnConcreteTypeByRef) //
				);
#endif

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
#endif
	}

	void explicitStubbingDefualtReturnValues() {
		Mock<ReferenceInterface> mock;		//
		When(Method(mock,returnIntByRef)).Return(); //
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		When(Method(mock,returnAbstractTypeByRef)).Return(); //
#endif
		When(Method(mock,returnConcreteTypeByRef)).Return(); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
#endif
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

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
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

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
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

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingDefualtReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;
		When(Method(mock,returnIntByRef)).AlwaysReturn();
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		When(Method(mock,returnAbstractTypeByRef)).AlwaysReturn(); //
#endif
		When(Method(mock,returnConcreteTypeByRef)).AlwaysReturn(); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
#endif
	}

} __ReferenceTypesTests;
