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
#include "fakeit/FakeitExceptions.hpp"

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
			Fake(mock[&ConstVolatileFunctions::func1]);
			Fake(mock[&ConstVolatileFunctions::func2]);
			Fake(mock[&ConstVolatileFunctions::func3]);
			Fake(mock[&ConstVolatileFunctions::func4]);
			Fake(mock[&ConstVolatileFunctions::func6]);
			Fake(mock[&ConstVolatileFunctions::func7]);

			Fake(mock[&ConstVolatileFunctions::proc1]);
			Fake(mock[&ConstVolatileFunctions::proc2]);
			Fake(mock[&ConstVolatileFunctions::proc3]);
//			Fake(mock[&ConstVolatileFunctions::proc4]);
// 			Fake(mock[&ConstVolatileFunctions::proc5));
// 			Fake(mock[&ConstVolatileFunctions::proc6));
// 			Fake(mock[&ConstVolatileFunctions::proc7));

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
//			i.proc4();
// 			i.proc5();
// 			i.proc6();
// 			i.proc7();
		}

		struct ConstVolatileParameters{
			virtual int func1(const int a, const volatile std::string s) = 0;
		};

		void TestConstParameters()
		{
			Mock<ConstVolatileParameters> mock;
			Fake(mock[&ConstVolatileParameters::func1]);
			ConstVolatileParameters& i = mock.get();
			ASSERT_EQUAL(0, i.func1(1,std::string()));
		}
} __RemoveConstVolatileTests;
