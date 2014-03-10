#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "fakeit/Exceptions.h"

using namespace fakeit;

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

//static bool operator==(const ReferenceInterface& a, const ReferenceInterface& b) {
//	return (&a == &b);
//}

struct ReferenceTypesTests: tpunit::TestFixture {
	ReferenceTypesTests() :
			tpunit::TestFixture(
			//
					TEST(ReferenceTypesTests::implicitStubbingDefaultReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefualtReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefualtReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues)
					//
							) //
	{
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
		Mock<ReferenceInterface> mock;
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
		Mock<ReferenceInterface> mock;//

		int num{1};
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
