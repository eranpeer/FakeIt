#include "stdafx.h"
#include "CppUnitTest.h"
#include "fakeit.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stubbing_tests
{
	TEST_CLASS(RemoveConstVolatileTests)
	{
	public:

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

		TEST_METHOD(TestConstFunctions)
		{
			Mock<ConstVolatileFunctions> mock;
			//mock.getA().foo();
			Stub(mock[&ConstVolatileFunctions::func1]);
			Stub(mock[&ConstVolatileFunctions::func2]);
			Stub(mock[&ConstVolatileFunctions::func3]);
			Stub(mock[&ConstVolatileFunctions::func4]);
			Stub(mock[&ConstVolatileFunctions::func6]);
			Stub(mock[&ConstVolatileFunctions::func7]);

			Stub(mock[&ConstVolatileFunctions::proc1]);
			Stub(mock[&ConstVolatileFunctions::proc2]);
			Stub(mock[&ConstVolatileFunctions::proc3]);
//			Stub(mock[&ConstVolatileFunctions::proc4]);
// 			Stub(mock[&ConstVolatileFunctions::proc5));
// 			Stub(mock[&ConstVolatileFunctions::proc6));
// 			Stub(mock[&ConstVolatileFunctions::proc7));

			ConstVolatileFunctions& i = mock.get();
			Assert::AreEqual(0, i.func1());
			Assert::AreEqual(0, i.func2());
			Assert::AreEqual(0, i.func3());
			Assert::AreEqual(0, i.func4());
			Assert::AreEqual(0, i.func6());
			Assert::AreEqual(0, i.func7());
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

		TEST_METHOD(TestConstParameters)
		{
			Mock<ConstVolatileParameters> mock;
			Stub(mock[&ConstVolatileParameters::func1]);
			ConstVolatileParameters& i = mock.get();
			Assert::AreEqual(0, i.func1(1,std::string()));
		}
	};
}