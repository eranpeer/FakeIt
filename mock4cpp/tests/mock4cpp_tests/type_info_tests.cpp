#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "mock4cpp.h"
#include "mockutils/Formatter.h"

struct TypeInfoTests: tpunit::TestFixture {
	TypeInfoTests() :
			tpunit::TestFixture(
			//
					TEST(TypeInfoTests::mock_should_use_same_typeid_as_moked_class) //
							)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
	};

	void mock_should_use_same_typeid_as_moked_class() {
		Mock<SomeInterface> mock;
		ASSERT_EQUAL(typeid(mock.get()),typeid(SomeInterface));
	}

} __TypeInfoTests;
