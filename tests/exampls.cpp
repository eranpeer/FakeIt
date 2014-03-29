#include <string>
#include <queue>
#include <iostream>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "fakeit/FakeitExceptions.h"

using namespace fakeit;
using namespace std;

struct DemoTests
//		: tpunit::TestFixture
		  {
	DemoTests()
//	:
//			tpunit::TestFixture(
			//
					//TEST(DemoTests::basic_stubbing), //
					//TEST(DemoTests::basic_verification) //
							//
//							)
		{
	}

	struct SomeInterface {
		virtual int foo(int) = 0;
		virtual int bar(string) = 0;
		virtual void proc(int) = 0;
	};

	void basic_stubbing() {
		// Instantiate a mock object.
		Mock<SomeInterface> mock;

		// Setup mock behavior.
		When(mock[&SomeInterface::foo]).AlwaysReturn(1);

		auto agrument_a_is_even = [](int a){return a%2==0;};
		When(mock[&SomeInterface::foo].Matching(agrument_a_is_even)).Return(0);

		// Fetch the mock instance.
		SomeInterface &i = mock.get();

		int a = i.foo(1);
		int a1 = i.foo(2);
		int a2 = i.foo(3);
//

		// Will print "1". 
		cout << i.foo(0);
	}

	void basic_verification() {
		Mock<SomeInterface> mock;
		//
		When(mock[&SomeInterface::foo]).AlwaysReturn(0);
		When(mock[&SomeInterface::bar]).AlwaysReturn(0);

		SomeInterface &i = mock.get();

		// Production code
		i.foo(1);
		i.bar("some value");

		// Verify for foo & bar where invoked
		Verify(mock[&SomeInterface::foo]);
		Verify(mock[&SomeInterface::bar]);

		// Verify for foo & bar where invoked with specific arguments
		Verify(mock[&SomeInterface::foo].Using(1));
		Verify(mock[&SomeInterface::bar].Using("some value"));

		// Verify for foo & bar where never invoked with other arguments
		Verify(mock[&SomeInterface::foo].Using(2)).Never();
		Verify(mock[&SomeInterface::bar].Using("some other value")).Never();
	}//

} __DemoTests;
