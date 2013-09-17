#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
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
			virtual const int func4() const volatile = 0;

			virtual void proc1() const = 0;
			virtual void proc2() volatile = 0;
			virtual void proc3() const volatile = 0;
			virtual const void proc4() const volatile = 0;

		};

		TEST_METHOD(TestConstFunctions)
		{
			Mock<ConstVolatileFunctions> mock;
			mock.Stub(&ConstVolatileFunctions::func1);
			mock.Stub(&ConstVolatileFunctions::func2);
			mock.Stub(&ConstVolatileFunctions::func3);
			mock.Stub(&ConstVolatileFunctions::func4);
			mock.Stub(&ConstVolatileFunctions::proc1);
			mock.Stub(&ConstVolatileFunctions::proc2);
			mock.Stub(&ConstVolatileFunctions::proc3);
			mock.Stub(&ConstVolatileFunctions::proc4);
			ConstVolatileFunctions& i = mock.get();
			Assert::AreEqual(0, i.func1());
			Assert::AreEqual(0, i.func2());
			Assert::AreEqual(0, i.func3());
			Assert::AreEqual(0, i.func4());
			i.proc1();
			i.proc2();
			i.proc3();
			i.proc4();
		}

	};
}