#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
#include <iostream>
#include <functional>
#include <tuple>
#include "TupleDispatcher.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{

	TEST_CLASS(StubbingTests)
	{
	public:

		struct SomeInterface
		{
			virtual int func(int) = 0;
			virtual void proc(int) = 0;
		};


		TEST_METHOD(UseInvocationMatcher)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func).Matching([](int a){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc).Matching([](int a){return a == 1; }).Throw(std::string("impl"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
		}
	};
}

