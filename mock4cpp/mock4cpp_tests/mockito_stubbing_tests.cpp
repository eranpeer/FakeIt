#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{

	TEST_CLASS(MockitoStubbingTests)
	{
	public:
		struct SomeInterface
		{
			virtual int func(int) = 0;
			virtual void proc(int) = 0;
		};

		TEST_METHOD(StubAllCallsToAlternateBeavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func, &SomeInterface::proc);

// 			When(mock().func(1)).Return(1);
// 			When(mock().func(2)).Return(1);

// 			SomeInterface &i = mock.get();
// 
// 			Assert::AreEqual(1, i.func(1));
// 			Assert::AreEqual(2, i.func(2));
		}
	};

}
