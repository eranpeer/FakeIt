#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "fakeit/Exceptions.h"

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
		Fake( //
				mock[&ReferenceInterface::returnIntByRef], //
				mock[&ReferenceInterface::returnAbstractTypeByRef], //
				mock[&ReferenceInterface::returnConcreteTypeByRef] //
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

	void explicitStubbingDefualtReturnValues() {
		Mock<ReferenceInterface> mock;		//
		When(mock[&ReferenceInterface::returnIntByRef]).Return(); //
		When(mock[&ReferenceInterface::returnAbstractTypeByRef]).Return(); //
		When(mock[&ReferenceInterface::returnConcreteTypeByRef]).Return(); //

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
		When(mock[&ReferenceInterface::returnIntByRef]).Return(num); //
		When(mock[&ReferenceInterface::returnConcreteTypeByRef]).Return(c); //
		When(mock[&ReferenceInterface::returnAbstractTypeByRef]).Return(a); //

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

	void explicitStubbingReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		When(mock[&ReferenceInterface::returnIntByRef]).AlwaysReturn(num); //
		When(mock[&ReferenceInterface::returnConcreteTypeByRef]).AlwaysReturn(c); //
		When(mock[&ReferenceInterface::returnAbstractTypeByRef]).AlwaysReturn(a); //

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

	void explicitStubbingReturnValues_by_assignment() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		mock[&ReferenceInterface::returnIntByRef] = num;
		mock[&ReferenceInterface::returnConcreteTypeByRef] = c;
		mock[&ReferenceInterface::returnAbstractTypeByRef] = a;

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

	void explicitStubbingDefualtReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;
		When(mock[&ReferenceInterface::returnIntByRef]).AlwaysReturn();
		When(mock[&ReferenceInterface::returnAbstractTypeByRef]).AlwaysReturn(); //
		When(mock[&ReferenceInterface::returnConcreteTypeByRef]).AlwaysReturn(); //

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

