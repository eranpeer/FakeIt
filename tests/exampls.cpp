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
#include <utility>
#include <type_traits>

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

					TEST(DemoTests::test1) //
							)
		{
	}

	class Some {
	public:
		virtual ~Some(){}
		virtual void f1(){}
		virtual void f2(){}
	};

	struct SomeInterface {
		virtual int foo(int) = 0;
		virtual int bar(int, int) = 0;
	};


	void test1(){
//
		Mock<SomeInterface> m;
		When(Call(m,foo)).Return(0);
		ASSERT_EQUAL(0,m.get().foo(1));
	}

} __DemoTests;
