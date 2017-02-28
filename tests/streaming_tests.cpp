/*
 * Copyright (c) 2016 Oxford Nanopore Technologies
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on May 11, 2016
 */

#include <string>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct StreamingTests : tpunit::TestFixture {

	StreamingTests() :
	tpunit::TestFixture(
	TEST(StreamingTests::enum_class_arg)
	)
	{
	}
    
    enum class SomeEnum
    {
        Item1,
        Item2
    };

	struct SomeInterface {
		virtual void func(SomeEnum) = 0;
	};
    
    void enum_class_arg() {
        Mock<SomeInterface> mock;
        When(Method(mock, func)).AlwaysReturn();
        mock.get().func(SomeEnum::Item1);
        Verify(Method(mock, func)).Exactly(1);
    }

} __StreamingTests;
