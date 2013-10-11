#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
#include <iostream>

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



	TEST_CLASS(StubbingTests)
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
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::proc).Throw(std::string("error"));

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
			mock.Stub(&SomeInterface::func).Do(&defaultFuncBehavior);
			mock.Stub(&SomeInterface::proc).Do(&defaultProcBehavior);

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
			mock.Stub(&SomeInterface::func).Do([](int a){return a; });
			mock.Stub(&SomeInterface::proc).Do([](int a){throw a; });

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

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).When(1).Return(1);
			mock.Stub(&SomeInterface::proc).When(1).Throw(std::string("error"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(0, i.func(2), L"default behavior");

			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			i.proc(2);
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior_ByAssignment)
		{
			Mock<SomeInterface> mock;
			mock[&SomeInterface::func].When(1) = [](...){return 1; };
			mock[&SomeInterface::proc].When(1) = [](...){throw std::string("error"); };


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
			std::function<int(int) > funcStub = [](...){return 1; };
			std::function<void(int) > procStub = [](...){ throw 1; };

			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Do(funcStub);
			mock.Stub(&SomeInterface::proc).Do(procStub);

			mock.Stub(&SomeInterface::func).When(1).Return(2);
			mock.Stub(&SomeInterface::proc).When(1).Return();

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
			mock.Stub(&SomeInterface::func)
				.Return(1)
				.ThenReturn(2)
				.ThenDo([](...){return 3; })
				.ThenDo(&defaultFuncBehavior);
			mock.Stub(&SomeInterface::proc).Return()
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
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::func).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(10));
			Assert::AreEqual(2, i.func(11));
		}

		TEST_METHOD(RestubDefaultProcBehavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::proc).Throw(0);
			mock.Stub(&SomeInterface::proc).Return();

			SomeInterface &i = mock.get();

			i.proc(0); // should not throw exception
		}

		TEST_METHOD(RestubReturnValue)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).When(1).Return(1);
			mock.Stub(&SomeInterface::func).When(1).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(1));
		}


		TEST_METHOD(StubAllButOne)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::func).When(1).Return(2);

			mock.Stub(&SomeInterface::proc).Throw(0);
			mock.Stub(&SomeInterface::proc).When(1).Return();

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(0));
			Assert::AreEqual(2, i.func(1));

			Assert::ExpectException<int>([&i]{ i.proc(0); });
			i.proc(1);
		}

		TEST_METHOD(StubProcWithReferenceParams){
			Mock<ReferenceInterface> mock;
			mock.Stub(&ReferenceInterface::proc1);
			mock.Stub(&ReferenceInterface::proc2);
			mock.Stub(&ReferenceInterface::func1);
			mock.Stub(&ReferenceInterface::func2);

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
			mock.Stub(&PointerInterface::proc1);
			mock.Stub(&PointerInterface::proc2);
			mock.Stub(&PointerInterface::func1);
			mock.Stub(&PointerInterface::func2);
			PointerInterface & i = mock.get();
			i.proc1(nullptr);
			i.proc2(nullptr);
			Assert::IsNull(i.func1());
			Assert::IsNull(i.func2());
		}
	};
}


