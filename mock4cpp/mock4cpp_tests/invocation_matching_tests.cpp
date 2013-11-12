#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{
	TEST_CLASS(invocation_matching_tests)
	{
	public:
		
		struct SomeInterface
		{
			virtual int func1(int) = 0;
			virtual void proc1(int) = 0;

			virtual int func2(int&) = 0;
			virtual void proc2(int&) = 0;

			virtual int func3(const int) = 0;
			virtual void proc3(const int) = 0;

		};


		TEST_METHOD(MatchInvocation_primitive_parameter)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func1).Matching([](int a){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc1).Matching([](int a){return a == 1; }).Throw(std::string("impl"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func1(1));
			Assert::ExpectException<std::string>([&i]{ i.proc1(1); });
		}

		TEST_METHOD(MatchInvocation_primitive_parameter_by_ref)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func2).Matching([](int a){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc2).Matching([](int a){return a == 1; }).Throw(std::string("impl"));

			SomeInterface &i = mock.get();
			int one = 1;
			Assert::AreEqual(2, i.func2(one));
			Assert::ExpectException<std::string>([&one,&i]{ i.proc2(one); });
		}

		TEST_METHOD(MatchInvocation_const_primitive_parameter)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func3).Matching([](int a){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc3).Matching([](int a){return a == 1; }).Throw(std::string("impl"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func3(1));
			Assert::ExpectException<std::string>([&i]{ i.proc3(1); });
		}

	};
}