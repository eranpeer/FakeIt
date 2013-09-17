#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stubbing_tests
{

	enum Color {RED = 1, GREEN = 2, BLUE = 3};

	struct ScalarFuctions {
		virtual bool boolFunc() = 0;
		virtual char charFunc() = 0;
		virtual char16_t char16Func() = 0;
		virtual char32_t char32Func() = 0;
		virtual wchar_t wcharFunc() = 0;
		virtual short shortFunc() = 0;
		virtual int intFunc() = 0;
		virtual long longFunc() = 0;
		virtual long long longLongFunc() = 0;
		virtual float floatFunc() = 0;
		virtual double doubleFunc() = 0;
		virtual long double longDoubleFunc() = 0;

		virtual Color enumFunc() = 0;

		virtual int * pIntFunc() = 0;
		virtual ScalarFuctions * pScalarFuctionsfunc() = 0;
		virtual std::nullptr_t nullptrFunc() = 0;

		typedef bool (ScalarFuctions::*func)();
		virtual func pMemberFunc() = 0;
	};

	struct DefaultConstructibleFunctions {
		virtual std::string stringfunc() = 0;
	};

	struct NotDefaultConstructible {
		NotDefaultConstructible(int a) : a(a){};
		const bool operator==(const NotDefaultConstructible &other) const {
			return a == other.a;
		}
	private:
		int a;
	};

	struct NonDefaultConstructibleFunctions {
		virtual NotDefaultConstructible notDefaultConstructibleFunc() = 0;
	};

	struct ReferenceFunctions {
		virtual int& scalarFunc() = 0;
		virtual std::string& stringFunc() = 0;
		virtual NotDefaultConstructible& notDefaultConstructibleFunc() = 0;
		virtual ReferenceFunctions& abstractTypeFunc() = 0;

		const bool operator==(const ReferenceFunctions &other) const {
			return this == &other;
		}

	};

	TEST_CLASS(DefaultBehaviorTests)
	{
	public:

		TEST_METHOD(DefaultBeaviorOfScalarFunctionsIsToReturnZero)
		{
			Mock<ScalarFuctions> mock;
			mock.Stub(&ScalarFuctions::boolFunc);
			mock.Stub(&ScalarFuctions::charFunc);
			mock.Stub(&ScalarFuctions::char16Func);
			mock.Stub(&ScalarFuctions::char32Func);
			mock.Stub(&ScalarFuctions::wcharFunc);
			mock.Stub(&ScalarFuctions::shortFunc);
			mock.Stub(&ScalarFuctions::intFunc);
			mock.Stub(&ScalarFuctions::longFunc);
			mock.Stub(&ScalarFuctions::longLongFunc);
			mock.Stub(&ScalarFuctions::floatFunc);
			mock.Stub(&ScalarFuctions::doubleFunc);
			mock.Stub(&ScalarFuctions::longDoubleFunc);
			mock.Stub(&ScalarFuctions::enumFunc);
			mock.Stub(&ScalarFuctions::pIntFunc);
			mock.Stub(&ScalarFuctions::pScalarFuctionsfunc);
			mock.Stub(&ScalarFuctions::nullptrFunc);
			mock.Stub(&ScalarFuctions::pMemberFunc);

			ScalarFuctions &i = mock.get();

			//Default behavior of a scalar function is to return 0/false/null

			Assert::AreEqual(false, i.boolFunc());
			Assert::AreEqual((char) 0, i.charFunc());
			Assert::AreEqual((int)(char16_t) 0, (int)i.char16Func());
			Assert::AreEqual((char32_t) 0, i.char32Func());
			Assert::AreEqual((wchar_t) 0, i.wcharFunc());
			Assert::AreEqual((short) 0, i.shortFunc());
			Assert::AreEqual((int) 0, i.intFunc());
			Assert::AreEqual((long) 0, i.longFunc());
			Assert::AreEqual((long)0, (long)i.longLongFunc());
			Assert::AreEqual((float) 0, i.floatFunc());
			Assert::AreEqual((double) 0, i.doubleFunc());
			Assert::AreEqual((double) 0, (double) i.longDoubleFunc());
			Assert::AreEqual(0, (int) i.enumFunc());
			Assert::IsNull(i.pIntFunc());
			Assert::IsNull(i.pScalarFuctionsfunc());
			Assert::AreEqual((int) nullptr, (int) i.nullptrFunc());
			Assert::AreEqual(0, union_cast<int>(i.pMemberFunc()));
		}

		struct VoidFunctions
		{
			virtual void proc1() = 0;
			virtual void proc2(int a) = 0;
		};

		TEST_METHOD(DefaultBeaviorOfVoidFunctionsIsToDoNothing)
		{
			Mock<VoidFunctions> mock;
			mock.Stub(&VoidFunctions::proc1);
			mock.Stub(&VoidFunctions::proc2);
			VoidFunctions& i = mock.get();
			i.proc1();
			i.proc2(1);
		}

		TEST_METHOD(ReturnByValue_ReturnDefaultConstructedObject)
		{
			Mock<DefaultConstructibleFunctions> mock;
			mock.Stub(&DefaultConstructibleFunctions::stringfunc);
			DefaultConstructibleFunctions& i = mock.get();
			Assert::AreEqual(std::string(), i.stringfunc());
		}

		TEST_METHOD(ReturnByReference_ReturnReferenceToDefaultConstructedObject)
		{
			Mock<ReferenceFunctions> mock;
			mock.Stub(&ReferenceFunctions::scalarFunc);
			mock.Stub(&ReferenceFunctions::stringFunc);
			ReferenceFunctions& i = mock.get();
			Assert::AreEqual(0, i.scalarFunc());
			Assert::AreEqual(std::string(), i.stringFunc());
		}

		TEST_METHOD(ReturnByValue_ThrowExceptionIfNotDefaultConstructible)
		{
			Mock<NonDefaultConstructibleFunctions> mock;
			mock.Stub(&NonDefaultConstructibleFunctions::notDefaultConstructibleFunc);
			NonDefaultConstructibleFunctions& i = mock.get();
			Assert::ExpectException<std::string>([&i]{ i.notDefaultConstructibleFunc(); });
		}

		TEST_METHOD(ReturnByReference_ThrowExceptionIfNotDefaultConstructible)
		{
			Mock<ReferenceFunctions> mock;
			mock.Stub(&ReferenceFunctions::notDefaultConstructibleFunc);
			ReferenceFunctions& i = mock.get();
			Assert::ExpectException<std::string>([&i]{ i.notDefaultConstructibleFunc(); }, 
				L"should fail to create default value");
		}

		TEST_METHOD(ReturnByReference_ThrowExceptionIfAbstract)
		{
			Mock<ReferenceFunctions> mock;
			mock.Stub(&ReferenceFunctions::abstractTypeFunc);
			ReferenceFunctions& i = mock.get();
			Assert::ExpectException<std::string>([&i]{ i.abstractTypeFunc(); },
				L"should fail to create default value");
		}

		TEST_METHOD(OverrideDefualtBehavior_Scalar)
		{
			Mock<ScalarFuctions> mock;
			mock.Stub(&ScalarFuctions::boolFunc).Return(true);
			mock.Stub(&ScalarFuctions::doubleFunc).Return(1.1);
			ScalarFuctions& i = mock.get();
			Assert::IsTrue(i.boolFunc());
			Assert::AreEqual(1.1, i.doubleFunc());
		}

		TEST_METHOD(OverrideDefualtBehavior_NotDefaultConstructible)
		{
			Mock<NonDefaultConstructibleFunctions> mock;
			mock.Stub(&NonDefaultConstructibleFunctions::notDefaultConstructibleFunc).Return(NotDefaultConstructible(1));
			NonDefaultConstructibleFunctions& i = mock.get();
			Assert::IsTrue(NotDefaultConstructible(1) == i.notDefaultConstructibleFunc());
			Assert::IsFalse(NotDefaultConstructible(2) == i.notDefaultConstructibleFunc());
		}

		TEST_METHOD(OverrideDefualtBehavior_AbstractType)
		{
			Mock<ReferenceFunctions> mock;
			ReferenceFunctions* pAbstractType{ nullptr };
			ReferenceFunctions& refAbstractType{ *pAbstractType };
			mock.Stub(&ReferenceFunctions::abstractTypeFunc).Return(refAbstractType);
			ReferenceFunctions& i = mock.get();
			Assert::IsNull(&i.abstractTypeFunc());
 		}

		struct SomeInterface {
			virtual void proc(int a) = 0;
			virtual int func(int a) = 0;
		};

		TEST_METHOD(DefualtBehaviorMatchAllInvocations)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::proc);
			mock.Stub(&SomeInterface::func);
			SomeInterface& i = mock.get();
			i.proc(1);
			i.proc(2);
			Assert::AreEqual(0, i.func(1));
			Assert::AreEqual(0, i.func(2));
		}

		TEST_METHOD(OverideDefualtBehaviorMatchAllInvocations)
		{
			Mock<SomeInterface> mock;
			mock.Stub(&SomeInterface::proc).Throw(std::string());
			mock.Stub(&SomeInterface::func).Return(1);
			SomeInterface& i = mock.get();
			
			Assert::ExpectException<std::string>([&](){i.proc(1); });
			Assert::ExpectException<std::string>([&](){i.proc(2); });
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(2));
		}

		union MyUnion
		{
			int a;
			double d;

			MyUnion() :a{0}{}
	
			const bool operator==(const MyUnion& other) const {
				return other.a == a;
			}
		};

		struct UnionFunctions
		{
			virtual MyUnion unionFunc() = 0;
			virtual MyUnion& unionRefFunc() = 0;
		};

		TEST_METHOD(ReturnByValue_ReturnDefaultConstructedUnion)
		{
			Mock<UnionFunctions> mock;
			mock.Stub(&UnionFunctions::unionFunc);
			UnionFunctions& i = mock.get();
			Assert::IsTrue(MyUnion() == i.unionFunc());
		}

		TEST_METHOD(ReturnByReference_ReturnReferenceToDefaultConstructedUnion)
		{
			Mock<UnionFunctions> mock;
			mock.Stub(&UnionFunctions::unionRefFunc);
			UnionFunctions& i = mock.get();
			Assert::IsTrue(MyUnion() == i.unionRefFunc());
		}

		struct ConstVolatileFunctions{
			virtual const int func1() const = 0;
			virtual const int func2() volatile = 0;
		};

		TEST_METHOD(TestConstFunctions)
		{
			Mock<ConstVolatileFunctions> mock;
			mock.Stub(&ConstVolatileFunctions::func1);
			mock.Stub(&ConstVolatileFunctions::func2);
			ConstVolatileFunctions& i = mock.get();
			Assert::AreEqual(0, i.func1());
			Assert::AreEqual(0, i.func2());
		}

	};
}