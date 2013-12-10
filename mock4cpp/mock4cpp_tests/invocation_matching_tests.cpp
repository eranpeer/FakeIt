#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>

#include "mock4cpp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{
	TEST_CLASS(invocation_matching_tests)
	{
	public:
		
		struct SomeInterface
		{
			virtual int func1(int, std::string) = 0;
			virtual void proc1(int, std::string) = 0;

			virtual int func2(int&, std::string&) = 0;
			virtual void proc2(int&, std::string&) = 0;

			virtual int func3(const int, const std::string&) = 0;
			virtual void proc3(const int, const std::string&) = 0;
		};

		TEST_METHOD(MatchInvocation_ByValue)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func1).Matching([](int a, std::string s){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc1).Matching([](int a, std::string s){return a == 1; }).Throw(std::string("exception"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func1(1,std::string("A")));
			Assert::ExpectException<std::string>([&]{ i.proc1(1, std::string("A")); });
		}

		TEST_METHOD(MatchInvocation_ByRef)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func2).Matching([](int a, std::string s){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc2).Matching([](int a, std::string s){return a == 1; }).Throw(std::string("exception"));

			SomeInterface &i = mock.get();
			int one = 1;
			std::string s{""};
			Assert::AreEqual(2, i.func2(one,s));
			Assert::ExpectException<std::string>([&]{ i.proc2(one,s); });
		}

		TEST_METHOD(MatchInvocation_const_primitive_parameter)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func3).Matching([](int a, const std::string &s){return a == 1; }).Return(2);
			mock.When(&SomeInterface::proc3).Matching([](int a, const std::string &s){return a == 1; }).Throw(std::string("exception"));

			// should also work
			mock.When(&SomeInterface::func3).Matching([](int a, std::string s){return a == 2; }).Return(2);
			mock.When(&SomeInterface::proc3).Matching([](int a, std::string s){return a == 2; }).Throw(std::string("exception"));

			SomeInterface &i = mock.get();
			std::string s{ "" };

			Assert::AreEqual(2, i.func3(1,s));
			Assert::ExpectException<std::string>([&]{ i.proc3(1,s); });

			Assert::AreEqual(2, i.func3(2, s));
			Assert::ExpectException<std::string>([&]{ i.proc3(2, s); });
		}

	};
}