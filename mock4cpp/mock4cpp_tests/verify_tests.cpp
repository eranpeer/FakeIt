#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
namespace mock4cpp_tests
{

	TEST_CLASS(VerificationTests)
	{
	public:

		struct SomeInterface
		{
			virtual int func(int) = 0;
			virtual void proc(int) = 0;
		};

		TEST_METHOD(VerifyZeroInvoaction)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func);
			mock.Stub(&SomeInterface::proc);

			SomeInterface &i = mock.get();

			mock[&SomeInterface::func].Verify(1).Never();
			mock[&SomeInterface::func].Verify(2).Never();

			mock[&SomeInterface::proc].Verify(1).Never();
			mock[&SomeInterface::proc].Verify(2).Never();
		}

		TEST_METHOD(VerifyAtLeastOnceIsTheDefaultBehavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func);
			mock.Stub(&SomeInterface::proc);

			SomeInterface &i = mock.get();

			i.func(1);
			i.func(2);

			i.proc(1);
			i.proc(2);

			mock[&SomeInterface::func].Verify(1);
			mock[&SomeInterface::func].Verify(2);
			
			mock[&SomeInterface::proc].Verify(1);
			mock[&SomeInterface::proc].Verify(2);
		}
	};
}

