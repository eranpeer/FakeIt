#include "stdafx.h"
#include "CppUnitTest.h"
#include "mock4cpp.h"
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
			mock.Stub(&ConstVolatileFunctions::func1);
			mock.Stub(&ConstVolatileFunctions::func2);
			mock.Stub(&ConstVolatileFunctions::func3);
			mock.Stub(&ConstVolatileFunctions::func4);
			mock.Stub(&ConstVolatileFunctions::func6);
			mock.Stub(&ConstVolatileFunctions::func7);

			mock.Stub(&ConstVolatileFunctions::proc1);
			mock.Stub(&ConstVolatileFunctions::proc2);
			mock.Stub(&ConstVolatileFunctions::proc3);
			mock.Stub(&ConstVolatileFunctions::proc4);
// 			mock.Stub(&ConstVolatileFunctions::proc5);
// 			mock.Stub(&ConstVolatileFunctions::proc6);
// 			mock.Stub(&ConstVolatileFunctions::proc7);

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
			i.proc4();
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
			mock.Stub(&ConstVolatileParameters::func1);
			ConstVolatileParameters& i = mock.get();
			Assert::AreEqual(0, i.func1(1,std::string()));
		}
	};
}