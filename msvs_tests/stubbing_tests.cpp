#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <functional>
#include <tuple>

#include "fakeit.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace mock4cpp_tests
{

	struct PointerInterface {
		virtual int* func1() = 0;
		virtual PointerInterface* func2() = 0;
		virtual void proc1(int*) = 0;
		virtual void proc2(PointerInterface *) = 0;
	};

	struct SomeInterface
	{
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
	};

	TEST_CLASS(StubbingTests)
	{
	public:

		TEST_METHOD(ExceptionWhenCallingANonStubbedMethod)
		{
			Mock<SomeInterface> mock;
			SomeInterface &i = mock.get();

			Assert::ExpectException<UnexpectedMethodCallException>([&i]{ i.func(1); });
			Assert::ExpectException<UnexpectedMethodCallException>([&i]{ i.proc(2); });
		}

		TEST_METHOD(StubAllCallsToAlternateBeavior)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).AlwaysReturn(1);
			When(mock[&SomeInterface::proc]).AlwaysThrow(std::string("error"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(2));

			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			Assert::ExpectException<std::string>([&i]{ i.proc(2); });
		}

		static int defaultFuncBehavior(int a){
			return a;
		}

		static void defaultProcBehavior(int a){
			throw a;
		}

		TEST_METHOD(StubDefaultBehaviorWithStaticMethod)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).Do(&defaultFuncBehavior);
			When(mock[&SomeInterface::proc]).Do(&defaultProcBehavior);

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::ExpectException<int>([&i]{ i.proc(1); });
		}

		TEST_METHOD(StubDefaultBehaviorWithStaticMethod_ByAssignment)
		{
			Mock<SomeInterface> mock;

			mock[&SomeInterface::func] = &defaultFuncBehavior;
			mock[&SomeInterface::proc] = &defaultProcBehavior;

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::ExpectException<int>([&i]{ i.proc(1); });
		}

		TEST_METHOD(StubDefaultBehaviorWithLambda)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).Do([](int a){return a; });
			When(mock[&SomeInterface::proc]).Do([](int a){throw a; });

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::ExpectException<int>([&i]{ i.proc(1); });
		}

		TEST_METHOD(StubDefaultBehaviorWithLambda_ByAssignment)
		{
			Mock<SomeInterface> mock;
			mock[&SomeInterface::func] = [](int a){return a; };
			mock[&SomeInterface::proc] = [](int a){throw a; };

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(1));

			Assert::ExpectException<int>([&i]{ i.proc(1); });
			Assert::ExpectException<int>([&i]{ i.proc(1); });
		}

		TEST_METHOD(StubDefaultBehavior_MockitoStyle)
		{

			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).Do([](int a){return a; });
			When(mock[&SomeInterface::proc]).Do([](int a){throw a; });

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::ExpectException<int>([&i]{ i.proc(1); });
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior)
		{
			Mock<SomeInterface> mock;
			Fake(mock[&SomeInterface::func]);
			Fake(mock[&SomeInterface::proc]);

			When(mock[&SomeInterface::func].Using(1)).AlwaysReturn(1);
			When(mock[&SomeInterface::proc].Using(1)).AlwaysThrow(std::string("error"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(0, i.func(2), L"default behavior");

			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			i.proc(2);
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior_MockitoStyle)
		{
			Mock<SomeInterface> mock;
			Fake(mock[&SomeInterface::func]);
			Fake(mock[&SomeInterface::proc]);

			When(mock[&SomeInterface::func].Using(1)).AlwaysReturn(1);
			When(mock[&SomeInterface::proc].Using(1)).AlwaysThrow(std::string("error"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(0, i.func(2), L"default behavior");

			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			i.proc(2);
		}

		TEST_METHOD(LastMatchingStubUsed)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func].Using(1)).AlwaysReturn(1);
			When(mock[&SomeInterface::func]).AlwaysReturn(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
			Assert::AreEqual(2, i.func(2));
		}

		TEST_METHOD(LastMatchingStubUsed2)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func].Using(1)).Return(1);
			When(mock[&SomeInterface::func]).Return(2);
			When(mock[&SomeInterface::func].Using(1)).Return(3);

			SomeInterface &i = mock.get();

			Assert::AreEqual(3, i.func(1));
			Assert::AreEqual(2, i.func(2));
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior_ByAssignment)
		{
			Mock<SomeInterface> mock;

			Fake(mock[&SomeInterface::func]);
			Fake(mock[&SomeInterface::proc]);

			mock[&SomeInterface::func].Using(1) = [](...){return 1; };
			mock[&SomeInterface::proc].Using(1) = [](...){throw std::string("error"); };

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(0, i.func(2), L"default behavior");

			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			i.proc(2);
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior_WithExplicitDefaultBehavior)
		{
			std::function<int(int)> funcStub = [](...){return 1; };
			std::function<void(int)> procStub = [](...){ throw 1; };

			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).AlwaysDo(funcStub);
			When(mock[&SomeInterface::proc]).AlwaysDo(procStub);

			When(mock[&SomeInterface::func].Using(1)).AlwaysReturn(2);
			When(mock[&SomeInterface::proc].Using(1)).AlwaysReturn();

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
			Assert::AreEqual(2, i.func(1));
			Assert::AreEqual(1, i.func(2), L"default behavior");

			i.proc(1);
			i.proc(1);
			Assert::ExpectException<int>([&i]{ i.proc(2); });
		}


		TEST_METHOD(StubIteratorStyle)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func])
				.Return(1)
				.Return(2)
				.Do([](...){return 3; })
				.Do(&defaultFuncBehavior);
			When(mock[&SomeInterface::proc]).Return()
				.Throw(std::string("error"))
				.Return()
				.Do([](...){})
				.Do(&defaultProcBehavior);

			SomeInterface &i = mock.get();

			Assert::IsTrue(1 == i.func(0));
			Assert::IsTrue(2 == i.func(0));
			Assert::IsTrue(3 == i.func(0));
			Assert::IsTrue(4 == i.func(4));

			i.proc(0);
			Assert::ExpectException<std::string>([&i]{ i.proc(0); });
			i.proc(0);
			i.proc(0);
			Assert::ExpectException<int>([&i]{ i.proc(0); });
		}

		TEST_METHOD(RestubDefaultReturnValue)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).AlwaysReturn(1);
			When(mock[&SomeInterface::func]).AlwaysReturn(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(10));
			Assert::AreEqual(2, i.func(11));
		}

		TEST_METHOD(RestubDefaultProcBehavior)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::proc]).Throw(0);
			When(mock[&SomeInterface::proc]).Return();

			SomeInterface &i = mock.get();

			i.proc(0); // should not throw exception
		}

		TEST_METHOD(RestubReturnValue)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func].Using(1)).Return(1);
			When(mock[&SomeInterface::func].Using(1)).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
		}


		TEST_METHOD(StubAllButOne)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func]).Return(1);
			When(mock[&SomeInterface::func].Using(1)).Return(2);

			When(mock[&SomeInterface::proc]).Throw(0);
			When(mock[&SomeInterface::proc].Using(1)).Return();

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(0));
			Assert::AreEqual(2, i.func(1));

			Assert::ExpectException<int>([&i]{ i.proc(0); });
			i.proc(1);
		}

		TEST_METHOD(StubProcWithPointerParams){
			Mock<PointerInterface> mock;
			Fake(mock[&PointerInterface::proc1]);
			Fake(mock[&PointerInterface::proc2]);
			Fake(mock[&PointerInterface::func1]);
			Fake(mock[&PointerInterface::func2]);
			PointerInterface & i = mock.get();
			i.proc1(nullptr);
			i.proc2(nullptr);
			Assert::IsNull(i.func1());
			Assert::IsNull(i.func2());
		}

		TEST_METHOD(UseInvocationMatcher)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::func].Matching([](int a){return a == 1; })).Return(2);
			When(mock[&SomeInterface::proc].Matching([](int a){return a == 1; })).Throw(std::string("impl"));
			
			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
		}
	};
}
