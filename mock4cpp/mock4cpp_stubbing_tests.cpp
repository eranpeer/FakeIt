#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace mock4cpp_tests
{

	TEST_CLASS(MockTest)
	{
	public:	
		
		struct SomeInterface
		{
			virtual int func(int) = 0;
			virtual void proc(int) = 0;
		};

		TEST_METHOD(StubAllCallsToDefaultBeavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func);
			mock.Stub(&SomeInterface::proc);

			SomeInterface &i = mock.get();

			//Default behavior of a function is to return 0/false/null/default ctor
			Assert::AreEqual(0, i.func(1));
			Assert::AreEqual(0, i.func(2));

			// Default behavior of a procedure is to do nothing
			i.proc(1);
			i.proc(2);
		}

		TEST_METHOD(StubAllCallsToAlternateBeavior)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::func).Return(1);
			mock.Stub(&SomeInterface::proc).Throw(std::string("error"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(1, i.func(1), L"should return 1");
			Assert::AreEqual(1, i.func(2), L"should return 1");

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
			mock.Stub(&SomeInterface::proc).When(1).Throw(std::string("error"));

			SomeInterface &i = mock.get();

			Assert::AreEqual(1,i.func(1));
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(0, i.func(2),L"default behavior");

			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
			Assert::ExpectException<std::string>([&i]{ i.proc(1); });
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
			mock.Stub(&SomeInterface::proc).Return().ThenThrow(std::string("error")).ThenReturn();

			SomeInterface &i = mock.get();

			Assert::IsTrue(1 == i.func(0));
			Assert::IsTrue(2 == i.func(0));
			Assert::IsTrue(3 == i.func(0));

			i.proc(0);
			Assert::ExpectException<std::string>([&i]{ i.proc(0); });
			i.proc(0);
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

		struct AbstractClass
		{
			AbstractClass() : c(1)
// 				, s(1), i(1), f(1), d(1), l(1)
			{}
 			
			char c;
// 			short s;
// 			int i;
// 			float f;
// 			double d;
// 			long l;

			virtual void proc() = 0;
		};

		TEST_METHOD(InitDataMembersToZero){
			Mock<AbstractClass> mock;
			AbstractClass &i = mock.get();
			Assert::AreEqual((char) 0, i.c);
// 			Assert::AreEqual((short)0, i.s);
// 			Assert::AreEqual((int) 0, i.i);
// 			Assert::AreEqual((float) 0, i.f);
// 			Assert::AreEqual((double) 0, i.d);
// 			Assert::AreEqual((long) 0, i.l);
		}

		struct ReferenceInterface {
			virtual int& func1() = 0;
			virtual ReferenceInterface& func2() = 0;

			virtual void proc1(int&) = 0;
			virtual void proc2(ReferenceInterface &) = 0;

			//bool operator == (const ReferenceInterface& other){return this == &other;}
		};
		


		TEST_METHOD(StubProcWithReferenceParams){
			Mock<ReferenceInterface> mock;
			mock.Stub(&ReferenceInterface::proc1);
			//int  a;
			//mock.Stub(&ReferenceInterface::proc2);
		}

		template<typename... arglist>
		struct ArgsHolder {

			ArgsHolder(const arglist&... args) : tuple(args...){
			}
 			
			bool match(const arglist& ... args){
				return tuple == std::tuple<arglist...>(args...);
			}

		private:
			std::tuple<arglist...> tuple;
		};

		void f(const int&& a){
		}

		void f(const int & a){
		}

		TEST_METHOD(TestTuple){
			int a = 1;
			int& ar = a;
			int b = 1;
			int& br = b;

			f(ar);
 			ArgsHolder<int, int> ah(ar, a);
 			Assert::IsTrue(ah.match(1, 1));
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
		
	};
}