#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "multiple_translation_units_stub.h"

using namespace fakeit;

struct MultipleTranslationUnitsStub : tpunit::TestFixture {
	MultipleTranslationUnitsStub()
		: tpunit::TestFixture(
			TEST(MultipleTranslationUnitsStub::NoCollidingIds)
			)
	{}

	void NoCollidingIds() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		
		stubFunc2(mock);
		When(Method(mock, func)).Return<int>(1);

		mock.get().func2(); // Uncatchable write access violation if ids collide
	}

} __MultipleTranslationUnitsStub;
