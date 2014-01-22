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
			Stub(mock[&SomeInterface::func1]);
			Stub(mock[&SomeInterface::func2]);
			Stub(mock[&SomeInterface::func3]);
			Stub(mock[&SomeInterface::proc1]);
			Stub(mock[&SomeInterface::proc2]);
			Stub(mock[&SomeInterface::proc3]);

			SomeInterface &i = mock.get();

			i.func1(1);
			i.func2(1);
			i.func2(2);

			i.proc1(1);
			i.proc2(1);
			i.proc2(2);

			Verify(mock[&SomeInterface::func1]);
			Verify(mock[&SomeInterface::func2]);
			Assert::ExpectException<MethodCallVerificationException>([&mock]{ Verify(mock[&SomeInterface::func3]); });


			Verify(mock[&SomeInterface::proc1]);
			Verify(mock[&SomeInterface::proc2]);
			Assert::ExpectException<MethodCallVerificationException>([&mock]{ Verify(mock[&SomeInterface::proc3]); });
		}

		TEST_METHOD(VerifyMethodInvocationCount)
		{
			Mock<SomeInterface> mock;
			Stub(mock[&SomeInterface::func1], mock[&SomeInterface::proc1]);

			SomeInterface &i = mock.get();

			i.func1(1);
			i.func1(2);

			Verify(mock[&SomeInterface::func1]).Twice();
			Verify(mock[&SomeInterface::func1].Using(1)).Once();
			Verify(mock[&SomeInterface::func1].Using(2)).Once();
			Verify(mock[&SomeInterface::func1].Using(3)).Never();

			Assert::ExpectException<MethodCallVerificationException>([&mock]{ Verify(mock[&SomeInterface::func1]).Never(); });
			Assert::ExpectException<MethodCallVerificationException>([&mock]{ Verify(mock[&SomeInterface::func1].Using(1)).Never(); });
		}

		TEST_METHOD(ExceptionOnNegativeVerifiedTimes)
		{
			Mock<SomeInterface> mock;
			SomeInterface &i = mock.get();
			Assert::ExpectException<IllegalArgumentException>([&mock]{ Verify(mock[&SomeInterface::func1]).Times(-1); });
			Assert::ExpectException<IllegalArgumentException>([&mock]{ Verify(mock[&SomeInterface::proc1]).Times(-1); });
		}

		TEST_METHOD(NewVerify)
		{
			Mock<SomeInterface> mock;
			Stub(mock[&SomeInterface::func1]);
			SomeInterface &i = mock.get();
			i.func1(1);
			i.func1(2);
			Verify(mock[&SomeInterface::func1] * 2);
		}


		TEST_METHOD(verify_concatenated_sequence) {
			Mock<SomeInterface> mock;
			Stub(mock[&SomeInterface::func1], mock[&SomeInterface::proc1]);
			SomeInterface &i = mock.get();

			i.func1(1);
			i.func1(2);
			i.func1(3);
			i.func1(4);

			Verify(mock[&SomeInterface::func1].Using(1) + mock[&SomeInterface::func1].Using(2)).Once();
			Verify(mock[&SomeInterface::func1].Using(2) + mock[&SomeInterface::func1].Using(3)).AtLeastOnce();
			Verify(mock[&SomeInterface::func1].Using(3) + mock[&SomeInterface::func1].Using(4)).Once();
			Verify(mock[&SomeInterface::func1] + mock[&SomeInterface::func1]).Twice();
			Verify(mock[&SomeInterface::func1].Using(1) + mock[&SomeInterface::func1].Using(3)).Never();

			Assert::ExpectException<MethodCallVerificationException>(
				[&mock]{Verify(mock[&SomeInterface::func1].Using(1) + mock[&SomeInterface::func1].Using(3)); });
		}

		TEST_METHOD(verify_repeated_sequence) {
			Mock<SomeInterface> mock;
			Stub(mock[&SomeInterface::func1], mock[&SomeInterface::proc1]);
			SomeInterface &i = mock.get();

			i.func1(1);
			i.func1(2);
			i.func1(3);
			i.func1(1);
			i.func1(2);
			i.func1(3);

			Verify(mock[&SomeInterface::func1] * 1).Times(6);
			Verify(mock[&SomeInterface::func1] * 2).Times(3);
			Verify(mock[&SomeInterface::func1] * 3).Times(2);
			Verify(mock[&SomeInterface::func1] * 4).Times(1);
			Verify(mock[&SomeInterface::func1] * 5).Times(1);
			Verify(mock[&SomeInterface::func1] * 6).Times(1);

			Verify(mock[&SomeInterface::func1].Using(1) + mock[&SomeInterface::func1].Using(2) + mock[&SomeInterface::func1].Using(3)).Twice();
			Verify((mock[&SomeInterface::func1].Using(1) + mock[&SomeInterface::func1].Using(2) + mock[&SomeInterface::func1].Using(3)) * 2).Once();
			Verify(mock[&SomeInterface::func1].Using(1) * 2).Never();

			Assert::ExpectException<MethodCallVerificationException>(
				[&mock]{Verify(mock[&SomeInterface::func1].Using(1) * 2); });
		}

		TEST_METHOD(verify_no_other_invocations) {
			Mock<SomeInterface> mock;
			Stub(mock[&SomeInterface::func1]);
			SomeInterface &i = mock.get();
			VerifyNoOtherInvocations(mock);

			i.func1(1);
			i.func1(1);
			Assert::ExpectException<MethodCallVerificationException>([&mock]{VerifyNoOtherInvocations(mock); });

			Verify(mock[&SomeInterface::func1]).AtLeastOnce();
			VerifyNoOtherInvocations(mock);

			i.func1(1);
			i.func1(1);
			Assert::ExpectException<MethodCallVerificationException>([&mock]{VerifyNoOtherInvocations(mock); });

			Verify(mock[&SomeInterface::func1] * 3);
			Assert::ExpectException<MethodCallVerificationException>([&mock]{VerifyNoOtherInvocations(mock); });

			Verify(mock[&SomeInterface::func1] * 4);
			VerifyNoOtherInvocations(mock);
		}

	};
}