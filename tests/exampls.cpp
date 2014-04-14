/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>
#include <iostream>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "fakeit/FakeitExceptions.hpp"

using namespace fakeit;
using namespace std;
using namespace tpunit;

struct DemoTests
		: tpunit::TestFixture
		  {
	DemoTests()
	:
			tpunit::TestFixture(

					TEST(DemoTests::on_other_invocations_verification) //
					//TEST(DemoTests::basic_verification) //
							//
							)
		{
	}

	struct SomeInterface {
		virtual int foo(int) = 0;
		virtual int bar(string) = 0;
		virtual void proc(int) = 0;
	};

	void basic_stubbing() {
	}

	void basic_verification() {
	}//

	void on_other_invocations_verification() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::foo]).AlwaysReturn(0);
		When(mock[&SomeInterface::bar]).AlwaysReturn(0);
		SomeInterface& i  = mock.get();
		i.foo(1);
		i.foo(2);
		i.bar("some string");

		Verify(mock[&SomeInterface::foo].Using(1));

		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException); // will fail.
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::foo]),fakeit::VerificationException); // will fail.

		Verify(mock[&SomeInterface::foo].Using(2));

		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException); // will fail.
		VerifyNoOtherInvocations(mock[&SomeInterface::foo]); // will pass.

		Verify(mock[&SomeInterface::bar]);
		VerifyNoOtherInvocations(mock); // will pass.
	}//

} __DemoTests;
