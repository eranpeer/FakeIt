/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct OverloadedMethods: tpunit::TestFixture {

	OverloadedMethods() :
	tpunit::TestFixture(
			TEST(OverloadedMethods::testOverloadedMEthods)
	) {
	}

	struct SomeInterface {
		virtual int func() = 0;
		virtual int func(int) = 0;
		virtual int func(int, std::string) = 0;
	};

	void testOverloadedMEthods(){
		Mock<SomeInterface> mock;
		int (SomeInterface::*void_arg)() = &SomeInterface::func;
		int (SomeInterface::*int_arg)(int) = &SomeInterface::func;
		int (SomeInterface::*int_string_arg)(int, std::string) = &SomeInterface::func;

		When(mock[void_arg]).Return(1);
		When(mock[int_arg]).Return(2);
		When(mock[int_string_arg]).Return(3);

		SomeInterface& i = mock.get();
		ASSERT_EQUAL(1,i.func());
		ASSERT_EQUAL(2,i.func(1));
		ASSERT_EQUAL(3,i.func(1,""));
	}

} __OverloadedMethods;
