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
					TEST(ReferenceTypesTests::defaultValuesOfReferenceTypes),
					TEST(ReferenceTypesTests::explicitStubbingDefualtReturnValuesOfReferenceTypes)
					//
							) //
	{
	}

	void defaultValuesOfReferenceTypes() {
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

	void explicitStubbingDefualtReturnValuesOfReferenceTypes() {
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

} __ReferenceTypesTests;
