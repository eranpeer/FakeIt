#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mock4cpp_tests
{
	
	TEST_CLASS(MockTest)
	{
	public:	
		
		struct PrimitiveFunctions
		{
			virtual bool fBool() = 0;
			virtual short fShort() = 0;
			virtual unsigned short fUnsignedShort() = 0;
			virtual int fInt() = 0;
			virtual unsigned int fUnsignedInt() = 0;
			virtual long fLong() = 0;
			virtual unsigned long fUnsignedLong() = 0;
			virtual float fFloat() = 0;
			virtual double fDouble() = 0;
		};

		struct SomeInterface
		{
			virtual int func(int) = 0;
			virtual void proc(int) = 0;
		};

		TEST_METHOD(StubWithoutWhenClouse_ShouldStubAllCallsToDefaultBeavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func);
			mock.Stub(&SomeInterface::proc);
			
			SomeInterface &i = mock.get();

			//Default behavior of a function is to return 0/false/null
			Assert::IsTrue(0 == i.func(1), L"should return 0");
			Assert::IsTrue(0 == i.func(2), L"should return 0");
			
			// Default behavior of a procedure is to do nothing
			i.proc(1);
			i.proc(2);
		}

		TEST_METHOD(StubAllCallsToDefaultBeavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func);
			mock.Stub(&SomeInterface::proc);

			SomeInterface &i = mock.get();

			//Default behavior of a function is to return 0/false/null
			Assert::IsTrue(0 == i.func(1), L"should return 0");
			Assert::IsTrue(0 == i.func(2), L"should return 0");

			// Default behavior of a procedure is to do nothing
			i.proc(1);
			i.proc(2);
		}

		TEST_METHOD(StubAllCallsToAlternateBeavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::proc).Throw();

			SomeInterface &i = mock.get();

			//Default behavior of a function is to return 0/false/null
			Assert::IsTrue(1 == i.func(1), L"should return 1");
			Assert::IsTrue(1 == i.func(2), L"should return 1");

			// Default behavior of a procedure is to do nothing
			Assert::ExpectException<char*>([&i]{ i.proc(1); }, L"should throw exception");
			Assert::ExpectException<char*>([&i]{ i.proc(2); }, L"should throw exception");
		}

		static int defaultFuncBehavior(int a){
			return 1;
		}

		static void defaultProcBehavior(int a){
			return;
		}

		TEST_METHOD(StubDefaultBehaviorWithStaticMethod)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Do(&defaultFuncBehavior);
			mock.Stub(&SomeInterface::proc).Do(&defaultProcBehavior);
			
			SomeInterface &i = mock.get();
			
			Assert::AreEqual(1, i.func(1));
			i.proc(1);
		}

		TEST_METHOD(StubDefaultBehaviorWithLambda)
		{
			std::function<int(int) > funcStub = [](int a){return a; };
			std::function<void(int) > procStub = [](int a){throw a; };

			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Do(funcStub);
			mock.Stub(&SomeInterface::proc).Do(procStub);
			
			SomeInterface &i = mock.get();
			
			Assert::AreEqual(1, i.func(1));
			Assert::ExpectException<int>([&i]{ i.proc(1); });
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).When(1).Return(1);
			mock.Stub(&SomeInterface::proc).When(1).Return();

			SomeInterface &i = mock.get();

			Assert::AreEqual(1,i.func(1));
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(0, i.func(2),L"default behavior");

			i.proc(1);
			i.proc(1);
			i.proc(2);
		}

		TEST_METHOD(StubOnlySpecifiedCallsToAlternateBehavior_WithExplicitDefaultBehavior)
		{
			std::function<int(int) > funcStub = [](int a){return 1; };
			std::function<void(int) > procStub = [](int a){ throw a; };

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
			mock.Stub(&SomeInterface::func).Return(1).ThenReturn(2).ThenReturn(3);
			mock.Stub(&SomeInterface::proc).Return().ThenThrow().ThenReturn();

			SomeInterface &i = mock.get();

			Assert::IsTrue(1 == i.func(0));
			Assert::IsTrue(2 == i.func(0));
			Assert::IsTrue(3 == i.func(0));

			i.proc(0);
			Assert::ExpectException<char*>([&i]{ i.proc(0); });
			i.proc(0);
		}

		TEST_METHOD(StubReturnValue)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::func).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(2, i.func(10));
			Assert::AreEqual(2, i.func(11));
		}

		TEST_METHOD(StubAllButOne)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::func).When(1).Return(2);

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(0));
			Assert::AreEqual(2, i.func(1));
		}


// 		TEST_METHOD(StubWithoutWhenClouse_ShouldStubAllCallsToDefaultBeaviour)
// 		{
// 			Mock<PrimitiveFunctions> mock;
// 			mock.Stub(&PrimitiveFunctions::fBool);
// 			mock.Stub(&PrimitiveFunctions::fShort);
// 			mock.Stub(&PrimitiveFunctions::fUnsignedShort);
// 			mock.Stub(&PrimitiveFunctions::fInt);
// 			mock.Stub(&PrimitiveFunctions::fUnsignedInt);
// 			mock.Stub(&PrimitiveFunctions::fLong);
// 			mock.Stub(&PrimitiveFunctions::fUnsignedLong);
// 			mock.Stub(&PrimitiveFunctions::fFloat);
// 			mock.Stub(&PrimitiveFunctions::fDouble);
// 
// 			PrimitiveFunctions &i = mock.get();
// 			Assert::IsTrue(false == i.fBool());
// 			Assert::IsTrue(0 == i.fShort());
// 			Assert::IsTrue(0 == i.fUnsignedShort());
// 			Assert::IsTrue(0 == i.fInt());
// 			Assert::IsTrue(0 == i.fUnsignedInt());
// 			Assert::IsTrue(0 == i.fLong());
// 			Assert::IsTrue(0 == i.fUnsignedLong());
// 			Assert::IsTrue(0 == i.fFloat());
// 			Assert::IsTrue(0 == i.fDouble());
// 		}



		struct VoidInterface
		{
			virtual void foo() = 0;
			virtual void bar(int a) = 0;
		};

		TEST_METHOD(CanMockVoidMethod)
		{
			Mock<VoidInterface> mock;
			mock.Stub(&VoidInterface::foo);
			mock.Stub(&VoidInterface::bar);
			VoidInterface &i = mock.get();
			i.foo();
			i.bar(1);
		}

		TEST_METHOD(StubMethodAndChangeReturnValue)
		{
			Mock<PrimitiveFunctions> mock;
			mock.Stub(&PrimitiveFunctions::fBool).When().Return(true);
			PrimitiveFunctions &i = mock.get();
			Assert::IsTrue(i.fBool());
		}

		TEST_METHOD(TestSyntax)
		{
			Mock<VoidInterface> mock;
			mock.Stub(&VoidInterface::foo).When().Return();
			mock.Stub(&VoidInterface::bar).When(1).Return();
			//mock.Stub(&VoidInterface::bar).when(1).thenThrow();
			VoidInterface &i = mock.get();
			i.foo();
			i.bar(1);

			Mock<PrimitiveFunctions> mock2;
			mock2.Stub(&PrimitiveFunctions::fBool).When().Return(true);
			mock2.Stub(&PrimitiveFunctions::fBool).When().Throw();
		}

	};
}