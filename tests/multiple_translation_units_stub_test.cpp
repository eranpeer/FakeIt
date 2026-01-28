#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "multiple_translation_units_stub.h"

using namespace fakeit;
using namespace multiple_tu;

#ifdef FAKEIT_CROSS_TRANSLATION_UNITS_MOCKS
struct MultipleTranslationUnitsStub : tpunit::TestFixture {

	MultipleTranslationUnitsStub() :
		tpunit::TestFixture(
			TEST(MultipleTranslationUnitsStub::NoCollidingIdsBasics),
			TEST(MultipleTranslationUnitsStub::NoCollidingIdsAlternateBasics),
			TEST(MultipleTranslationUnitsStub::NoCollidingIdsMoreFunctionsMocked),
			TEST(MultipleTranslationUnitsStub::NoCollidingIdsWhenOverridingMocks)
		)
	{
	}

	void NoCollidingIdsBasics()
	{
		Mock<SomeInterface> mock;

		stubFunc(mock);
		When(Method(mock, func2)).Return("Something");

		SomeInterface &i = mock.get();

		// Uncatchable write access violation if ids collide.
		EXPECT_EQUAL(i.func(5), 5);
		EXPECT_EQUAL(i.func2(5), "Something");

		Verify(Method(mock, func).Using(5)).Exactly(1);
		Verify(Method(mock, func2).Using(5)).Exactly(1);
	}

	void NoCollidingIdsAlternateBasics()
	{
		Mock<SomeInterface> mock;

		When(Method(mock, func)).Return(100);
		stubFunc2(mock);

		SomeInterface &i = mock.get();

		// Uncatchable write access violation if ids collide.
		EXPECT_EQUAL(i.func(5), 100);
		EXPECT_EQUAL(i.func2(5), "String");

		Verify(Method(mock, func).Using(5)).Exactly(1);
		Verify(Method(mock, func2).Using(5)).Exactly(1);
	}

	void NoCollidingIdsMoreFunctionsMocked()
	{
		Mock<SomeInterface> mock;

		stubFunc(mock);
		stubFunc2(mock);

		When(Method(mock, func).Using(20)).Return(20);
		When(Method(mock, func).Using(50)).Return(50);

		When(Method(mock, func2).Using(20)).Return("Something-20");
		When(Method(mock, func2).Using(50)).Return("Something-50");

		stubMoreFunc(mock);
		stubMoreFunc2(mock);

		SomeInterface &i = mock.get();

		// Uncatchable write access violation if ids collide.
		EXPECT_EQUAL(i.func(1), 10);
		EXPECT_EQUAL(i.func(2), 20);
		EXPECT_EQUAL(i.func(5), 5);
		EXPECT_EQUAL(i.func(20), 20);
		EXPECT_EQUAL(i.func(50), 50);
		EXPECT_EQUAL(i.func2(1), "String1");
		EXPECT_EQUAL(i.func2(2), "String2");
		EXPECT_EQUAL(i.func2(5), "String");
		EXPECT_EQUAL(i.func2(20), "Something-20");
		EXPECT_EQUAL(i.func2(50), "Something-50");

		Verify(Method(mock, func).Using(1)).Exactly(1);
		Verify(Method(mock, func).Using(2)).Exactly(1);
		Verify(Method(mock, func).Using(5)).Exactly(1);
		Verify(Method(mock, func).Using(20)).Exactly(1);
		Verify(Method(mock, func).Using(50)).Exactly(1);
		Verify(Method(mock, func2).Using(1)).Exactly(1);
		Verify(Method(mock, func2).Using(2)).Exactly(1);
		Verify(Method(mock, func2).Using(5)).Exactly(1);
		Verify(Method(mock, func2).Using(20)).Exactly(1);
		Verify(Method(mock, func2).Using(50)).Exactly(1);
	}

	void NoCollidingIdsWhenOverridingMocks()
	{
		Mock<SomeInterface> mock;

		stubFunc(mock);
		When(Method(mock, func)).Return(123);
		When(Method(mock, func2)).Return("Something");
		stubFunc2(mock);

		SomeInterface &i = mock.get();

		EXPECT_EQUAL(i.func(0), 123);
		EXPECT_EQUAL(i.func2(0), "String");

		try {
			i.func(0);
			FAIL();
		} catch (const UnexpectedMethodCallException&) {
			// There was only one action in the mock for this function.
		}

		try {
			i.func2(0);
			FAIL();
		} catch (const UnexpectedMethodCallException&) {
			// There was only one action in the mock for this function.
		}

		Verify(Method(mock, func)).Exactly(2);
		Verify(Method(mock, func)).Exactly(2);
	}

} __MultipleTranslationUnitsStub;
#endif
