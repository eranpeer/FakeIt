#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <functional>
#include <tuple>

#include "mock4cpp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{

	struct ReferenceInterface {
		virtual int& func1() = 0;
		virtual ReferenceInterface& func2() = 0;
		virtual void proc1(int&) = 0;
		virtual void proc2(ReferenceInterface &) = 0;
	};

	static bool operator==(const ReferenceInterface& a, const ReferenceInterface& b)
	{
		return (&a == &b);
	}

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

			Assert::ExpectException<UnmockedMethodCallException>([&i]{ i.func(1); });
			Assert::ExpectException<UnmockedMethodCallException>([&i]{ i.proc(2); });
		}

		TEST_METHOD(StubAllCallsToAlternateBeavior)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func).Return(1);
			mock.When(&SomeInterface::proc).Throw(std::string("error"));

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
			mock.When(&SomeInterface::func).Do(&defaultFuncBehavior);
			mock.When(&SomeInterface::proc).Do(&defaultProcBehavior);

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
			mock.When(&SomeInterface::func).Do([](int a){return a; });
			mock.When(&SomeInterface::proc).Do([](int a){throw a; });

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
			Stub(mock[&SomeInterface::func]);
			Stub(mock[&SomeInterface::proc]);

			mock.When(&SomeInterface::func).Using(1).Return(1);
			mock.When(&SomeInterface::proc).Using(1).Throw(std::string("error"));

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
			Stub(mock[&SomeInterface::func]);
			Stub(mock[&SomeInterface::proc]);

			When(mock[&SomeInterface::func].Using(1)).Return(1);
			When(mock[&SomeInterface::proc].Using(1)).Throw(std::string("error"));

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
			mock.When(&SomeInterface::func).Using(1).Return(1);
			mock.When(&SomeInterface::func).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
			Assert::AreEqual(2, i.func(2));
		}

		TEST_METHOD(LastMatchingStubUsed2)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func).Using(1).Return(1);
			mock.When(&SomeInterface::func).Return(2);
			mock.When(&SomeInterface::func).Using(1).Return(3);

			SomeInterface &i = mock.get();

			Assert::AreEqual(3, i.func(1));
			Assert::AreEqual(2, i.func(2));
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior_ByAssignment)
		{
			Mock<SomeInterface> mock;

			Stub(mock[&SomeInterface::func]);
			Stub(mock[&SomeInterface::proc]);

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
			mock.When(&SomeInterface::func).Do(funcStub);
			mock.When(&SomeInterface::proc).Do(procStub);

			mock.When(&SomeInterface::func).Using(1).Return(2);
			mock.When(&SomeInterface::proc).Using(1).Return();

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
			mock.When(&SomeInterface::func)
				.Return(1)
				.ThenReturn(2)
				.ThenDo([](...){return 3; })
				.ThenDo(&defaultFuncBehavior);
			mock.When(&SomeInterface::proc).Return()
				.ThenThrow(std::string("error"))
				.ThenReturn()
				.ThenDo([](...){})
				.ThenDo(&defaultProcBehavior);

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
			mock.When(&SomeInterface::func).Return(1);
			mock.When(&SomeInterface::func).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(10));
			Assert::AreEqual(2, i.func(11));
		}

		TEST_METHOD(RestubDefaultProcBehavior)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::proc).Throw(0);
			mock.When(&SomeInterface::proc).Return();

			SomeInterface &i = mock.get();

			i.proc(0); // should not throw exception
		}

		TEST_METHOD(RestubReturnValue)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func).Using(1).Return(1);
			mock.When(&SomeInterface::func).Using(1).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
		}


		TEST_METHOD(StubAllButOne)
		{
			Mock<SomeInterface> mock;
			mock.When(&SomeInterface::func).Return(1);
			mock.When(&SomeInterface::func).Using(1).Return(2);

			mock.When(&SomeInterface::proc).Throw(0);
			mock.When(&SomeInterface::proc).Using(1).Return();

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(0));
			Assert::AreEqual(2, i.func(1));

			Assert::ExpectException<int>([&i]{ i.proc(0); });
			i.proc(1);
		}

		TEST_METHOD(StubProcWithReferenceParams){
			Mock<ReferenceInterface> mock;
			Stub(mock[&ReferenceInterface::proc1], mock[&ReferenceInterface::proc2]);
			Stub(mock[&ReferenceInterface::func1], mock[&ReferenceInterface::func2]);

			ReferenceInterface & i = mock.get();
			ReferenceInterface* pReferenceInterface{ nullptr };
			ReferenceInterface& ref = *pReferenceInterface;
			int a{ 0 };
			i.proc1(a);
			i.proc2(ref);
			int& rInt = i.func1();
			Assert::AreEqual(0, rInt, L"fundamental types are initiated to 0");
			Assert::ExpectException<std::string>([&i]{ i.func2(); }, L"should fail to create default value");
		}

		TEST_METHOD(StubProcWithPointerParams){
			Mock<PointerInterface> mock;
			Stub(mock[&PointerInterface::proc1]);
			Stub(mock[&PointerInterface::proc2]);
			Stub(mock[&PointerInterface::func1]);
			Stub(mock[&PointerInterface::func2]);
			PointerInterface & i = mock.get();
			i.proc1(nullptr);
			i.proc2(nullptr);
			Assert::IsNull(i.func1());
			Assert::IsNull(i.func2());
		}

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
