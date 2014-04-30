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

					TEST(DemoTests::on_other_invocations_verification), //
					TEST(DemoTests::simple_inheritance_upcast), //
					TEST(DemoTests::simple_inheritance_dynamic_down_cast)
							//
							)
		{
	}

	struct SomeInterface {
		virtual int foo(int) = 0;
		virtual int bar(int, int) = 0;
	};

	void basic_stubbing() {
	}

	void basic_verification() {
	}//

	struct A {
		virtual int foo() = 0;
	};

	struct B : public A {
		virtual int foo() override = 0;
	};

	struct C : public B
	{
		virtual int foo() override = 0;
	};

	void simple_inheritance_upcast() {
		Mock<C> cMock;		
		When(cMock[&C::foo]).AlwaysReturn(0);
		
		C& c = cMock.get();
		B& b = c;
		A& a = b;

		ASSERT_EQUAL(0, c.foo());
		ASSERT_EQUAL(0, b.foo());
		ASSERT_EQUAL(0, a.foo());
	}

	void simple_inheritance_dynamic_down_cast() {
		Mock<C, B, A> cMock;
		When(cMock[&C::foo]).AlwaysReturn(0);
		A& a = cMock.get();

		B& b = dynamic_cast<B&>(a);
		ASSERT_EQUAL(0, b.foo());

		C& c = dynamic_cast<C&>(a);
		ASSERT_EQUAL(0, c.foo());
	}

	void on_other_invocations_verification() {
		Mock<SomeInterface> mock;
		// Stub a method to return a value once
		When(mock[&SomeInterface::foo]).Return(1);

		// Stub multiple return values (The next two lines do exactly the same)
		When(mock[&SomeInterface::foo]).Return(1, 2, 3);
		When(mock[&SomeInterface::foo]).Return(1).Return(2).Return(3);

		// Return the same value many times (56 in this example)
		When(mock[&SomeInterface::foo]).Return(Times<56>::of(1));

		// Return many values many times (First 100 calls will return 1, next 200 calls will return 2)
		When(mock[&SomeInterface::foo]).Return(Times<100>::of(1), Times<200>::of(2));

		// Always return a value (The next two lines do exactly the same)
		When(mock[&SomeInterface::foo]).AlwaysReturn(1);
		mock[&SomeInterface::foo] = 1;

		// Throw once
		When(mock[&SomeInterface::foo]).Throw(exception());
		// Throw several times
		When(mock[&SomeInterface::foo]).Throw(exception(), exception());
		// Throw many times
		When(mock[&SomeInterface::foo]).Throw(Times<23>::of(exception()));
		// Always throw
		When(mock[&SomeInterface::foo]).AlwaysThrow(exception());

		When(mock[&SomeInterface::foo]).AlwaysReturn(1);

		SomeInterface & i = mock.get();

		// Production code:
		i.foo(1);
		i.foo(2);
		i.foo(3);

		// Verify foo was invoked at least once. (The four lines do exactly the same)
		Verify(mock[&SomeInterface::foo]);
		Verify(mock[&SomeInterface::foo]).AtLeastOnce();
		Verify(mock[&SomeInterface::foo]).AtLeast(1);
		Verify(mock[&SomeInterface::foo]).AtLeast(Times<1>());

		// Verify foo was invoked at exactly 3 times. (The next two lines do exactly the same)
		Verify(mock[&SomeInterface::foo]).Exactly(3);
		Verify(mock[&SomeInterface::foo]).Exactly(Times<3>());

		// Verify foo(1) was invoked exactly once
		Verify(mock[&SomeInterface::foo].Using(1)).Once();
		Verify(mock[&SomeInterface::foo].Using(1)).Exactly(Once);

		// verify the actual invocation sequence contains two consecutive invocations of foo at least once.
		Verify(mock[&SomeInterface::foo] * 2);

		// verify the actual invocation sequence contains two consecutive invocations of foo exactly once.
		Verify(mock[&SomeInterface::foo] * 2).Exactly(Once);

		// verify the actual invocation sequence contains an invocation of foo(1) followed by bar(1,2) exactly twice.
		Verify(mock[&SomeInterface::foo].Using(1) + mock[&SomeInterface::bar].Using(1,2)).Exactly(Times<2>());
	}//

} __DemoTests;
