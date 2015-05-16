/*
* Copyright (c) 2014 Eran Pe'er.
*
* This program is made available under the terms of the MIT License.
*
* Created on Mar 10, 2014
*/

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct RemoveConstVolatileTests : tpunit::TestFixture {
public:

	RemoveConstVolatileTests() :
		tpunit::TestFixture(
		//
		TEST(RemoveConstVolatileTests::TestConstFunctions),
		TEST(RemoveConstVolatileTests::TestConstParameters)
		) {
		}

		struct ConstVolatileFunctions{
			virtual int func1() const = 0;
			virtual int func2() volatile = 0;
			virtual int func3() const volatile = 0;

			virtual const int func4() = 0;
			virtual const int func6() const = 0;
			virtual const int func7() const volatile = 0;

			virtual void proc1() const = 0;
			virtual void proc2() volatile = 0;
			virtual void proc3() const volatile = 0;
			virtual const void proc4() = 0;
 			virtual const void proc5() const = 0;
 			virtual const void proc6() volatile = 0;
 			virtual const void proc7() const volatile = 0;
 		};

		void TestConstFunctions()
		{
			Mock<ConstVolatileFunctions> mock;
			//mock.getA().foo();
			Fake(Method(mock,func1));
			Fake(Method(mock,func2));
			Fake(Method(mock,func3));
			Fake(Method(mock,func4));
			Fake(Method(mock,func6));
			Fake(Method(mock,func7));

			Fake(Method(mock,proc1));
			Fake(Method(mock,proc2));
			Fake(Method(mock,proc3));
			
//			bool v = std::is_void<const void>::value;

			Fake(Method(mock,proc4));
 			Fake(Method(mock,proc5));
 			Fake(Method(mock,proc6));
 			Fake(Method(mock,proc7));

			ConstVolatileFunctions& i = mock.get();
			ASSERT_EQUAL(0, i.func1());
			ASSERT_EQUAL(0, i.func2());
			ASSERT_EQUAL(0, i.func3());
			ASSERT_EQUAL(0, i.func4());
			ASSERT_EQUAL(0, i.func6());
			ASSERT_EQUAL(0, i.func7());
			i.proc1();
			i.proc2();
			i.proc3();
			i.proc4();
 			i.proc5();
 			i.proc6();
 			i.proc7();
		}

		struct ConstVolatileParameters{
			virtual int func1(const int a, const volatile std::string s) = 0;
		};

		void TestConstParameters()
		{
			Mock<ConstVolatileParameters> mock;
			Fake(Method(mock,func1));
			ConstVolatileParameters& i = mock.get();
			ASSERT_EQUAL(0, i.func1(1,std::string()));
		}

} __RemoveConstVolatileTests;
