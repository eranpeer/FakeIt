#include "stdafx.h"
#include "CppUnitTest.h"
#include "mock4cpp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{

	TEST_CLASS(VerificationTests)
	{
	public:

		struct SomeInterface
		{
			virtual int func1(int) = 0;
			virtual int func2(int) = 0;
			virtual int func3(int) = 0;
			virtual void proc1(int) = 0;
			virtual void proc2(int) = 0;
			virtual void proc3(int) = 0;
		};

		class A {
			int a;
		public:
			~A(){
				a++;
			}
		};

		TEST_METHOD(VerifyZeroInvoaction)
		{
			Mock<SomeInterface> mock;

			Verify(mock[&SomeInterface::func1]).Never();
			Verify(mock[&SomeInterface::func1].Using(1)).Never();
			Verify(mock[&SomeInterface::func1]).Times(0);
			Verify(mock[&SomeInterface::func1].Using(1)).Times(0);

			Verify(mock[&SomeInterface::proc1]).Never();
			Verify(mock[&SomeInterface::proc1].Using(1)).Never();
			Verify(mock[&SomeInterface::proc1]).Times(0);
			Verify(mock[&SomeInterface::proc1].Using(1)).Times(0);
		}

		TEST_METHOD(VerifyAtLeastOnceIsTheDefaultBehavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func1);
			mock.Stub(&SomeInterface::func2);
			mock.Stub(&SomeInterface::func3);
			mock.Stub(&SomeInterface::proc1);
			mock.Stub(&SomeInterface::proc2);
			mock.Stub(&SomeInterface::proc3);

			SomeInterface &i = mock.get();

			i.func1(1);
			i.func2(1);
			i.func2(2);

			i.proc1(1);
			i.proc2(1);
			i.proc2(2);

			Verify(mock[&SomeInterface::func1]);
			Verify(mock[&SomeInterface::func2]);
			Assert::ExpectException<AssertionException>([&mock]{ Verify(mock[&SomeInterface::func3]); });


			Verify(mock[&SomeInterface::proc1]);
			Verify(mock[&SomeInterface::proc2]);
			Assert::ExpectException<AssertionException>([&mock]{ Verify(mock[&SomeInterface::proc3]); });
		}

		TEST_METHOD(VerifyMethodInvocationCount)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func1, &SomeInterface::proc1);

			SomeInterface &i = mock.get();

			i.func1(1);
			i.func1(2);

			Verify(mock[&SomeInterface::func1]).Twice();
			Verify(mock[&SomeInterface::func1].Using(1)).Once();
			Verify(mock[&SomeInterface::func1].Using(2)).Once();
			Verify(mock[&SomeInterface::func1].Using(3)).Never();

			Assert::ExpectException<AssertionException>([&mock]{ Verify(mock[&SomeInterface::func1]).Never(); });
			Assert::ExpectException<AssertionException>([&mock]{ Verify(mock[&SomeInterface::func1].Using(1)).Never(); });
		}

		TEST_METHOD(ExceptionOnNegativeVerifiedTimes)
		{
			Mock<SomeInterface> mock;
			SomeInterface &i = mock.get();
			Assert::ExpectException<AssertionException>([&mock]{ Verify(mock[&SomeInterface::func1]).Times(-1); });
			Assert::ExpectException<AssertionException>([&mock]{ Verify(mock[&SomeInterface::proc1]).Times(-1); });
		}
	};
}