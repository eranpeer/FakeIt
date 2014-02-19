#include "stdafx.h"
#include "CppUnitTest.h"
#include "fakeit.h"
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

		TEST_METHOD(StabManyMethodsInOneCall)
		{
			Mock<ScalarFuctions> mock;
			Stub(mock[&ScalarFuctions::boolFunc]);
			Stub(mock[&ScalarFuctions::charFunc]);
			Stub(mock[&ScalarFuctions::intFunc]);

			ScalarFuctions &i = mock.get();

			Assert::AreEqual(false, i.boolFunc());
			Assert::AreEqual((char) 0, i.charFunc());
			Assert::AreEqual((int) 0, i.intFunc());
		}

		TEST_METHOD(DefaultBeaviorOfScalarFunctionsIsToReturnZero)
		{
			Mock<ScalarFuctions> mock;
			Stub(mock[&ScalarFuctions::boolFunc]);
			Stub(mock[&ScalarFuctions::charFunc]);
			Stub(mock[&ScalarFuctions::char16Func]);
			Stub(mock[&ScalarFuctions::char32Func]);
			Stub(mock[&ScalarFuctions::wcharFunc]);
			Stub(mock[&ScalarFuctions::shortFunc]);
			Stub(mock[&ScalarFuctions::intFunc]);
			Stub(mock[&ScalarFuctions::longFunc]);
			Stub(mock[&ScalarFuctions::longLongFunc]);
			Stub(mock[&ScalarFuctions::floatFunc]);
			Stub(mock[&ScalarFuctions::doubleFunc]);
			Stub(mock[&ScalarFuctions::longDoubleFunc]);
			Stub(mock[&ScalarFuctions::enumFunc]);
			Stub(mock[&ScalarFuctions::pIntFunc]);
			Stub(mock[&ScalarFuctions::pScalarFuctionsfunc]);
			Stub(mock[&ScalarFuctions::nullptrFunc]);
			Stub(mock[&ScalarFuctions::pMemberFunc]);

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
			Stub(mock[&VoidFunctions::proc1]);
			Stub(mock[&VoidFunctions::proc2]);
			VoidFunctions& i = mock.get();
			i.proc1();
			i.proc2(1);
		}

		TEST_METHOD(MultipleStubbingInOneLine)
		{
			Mock<ScalarFuctions> sMock;
			Mock<VoidFunctions> vMock;
			Stub(sMock[&ScalarFuctions::boolFunc]);
			Stub(vMock[&VoidFunctions::proc1]);
			Stub(sMock[&ScalarFuctions::charFunc], vMock[&VoidFunctions::proc2]);

			VoidFunctions& vi = vMock.get();
			vi.proc1();
			vi.proc2(1);

			ScalarFuctions& si = sMock.get();
			Assert::AreEqual(false, si.boolFunc());
			Assert::AreEqual((char) 0, si.charFunc());
		}

		TEST_METHOD(ReturnByValue_ReturnDefaultConstructedObject)
		{
			Mock<DefaultConstructibleFunctions> mock;
			Stub(mock[&DefaultConstructibleFunctions::stringfunc]);
			DefaultConstructibleFunctions& i = mock.get();
			Assert::AreEqual(std::string(), i.stringfunc());
		}

		TEST_METHOD(ReturnByReference_ReturnReferenceToDefaultConstructedObject)
		{
			Mock<ReferenceFunctions> mock;
			Stub(mock[&ReferenceFunctions::scalarFunc]);
			Stub(mock[&ReferenceFunctions::stringFunc]);
			ReferenceFunctions& i = mock.get();
			Assert::AreEqual(0, i.scalarFunc());
			Assert::AreEqual(std::string(), i.stringFunc());
		}

		TEST_METHOD(ReturnByValue_ThrowExceptionIfNotDefaultConstructible)
		{
			Mock<NonDefaultConstructibleFunctions> mock;
			Stub(mock[&NonDefaultConstructibleFunctions::notDefaultConstructibleFunc]);
			NonDefaultConstructibleFunctions& i = mock.get();
			Assert::ExpectException<std::string>([&i]{ i.notDefaultConstructibleFunc(); });
		}

		TEST_METHOD(ReturnByReference_ThrowExceptionIfNotDefaultConstructible)
		{
			Mock<ReferenceFunctions> mock;
			Stub(mock[&ReferenceFunctions::notDefaultConstructibleFunc]);
			ReferenceFunctions& i = mock.get();
			Assert::ExpectException<std::string>([&i]{ i.notDefaultConstructibleFunc(); }, 
				L"should fail to create default value");
		}

		TEST_METHOD(ReturnByReference_ThrowExceptionIfAbstract)
		{
			Mock<ReferenceFunctions> mock;
			Stub(mock[&ReferenceFunctions::abstractTypeFunc]);
			ReferenceFunctions& i = mock.get();
			Assert::ExpectException<std::string>([&i]{ i.abstractTypeFunc(); },
				L"should fail to create default value");
		}

		TEST_METHOD(OverrideDefualtBehavior_Scalar)
		{
			Mock<ScalarFuctions> mock;
			When(mock[&ScalarFuctions::boolFunc]).Return(true);
			When(mock[&ScalarFuctions::doubleFunc]).Return(1.1);
			ScalarFuctions& i = mock.get();
			Assert::IsTrue(i.boolFunc());
			Assert::AreEqual(1.1, i.doubleFunc());
		}

		TEST_METHOD(OverrideDefualtBehavior_NotDefaultConstructible)
		{
			Mock<NonDefaultConstructibleFunctions> mock;
			When(mock[&NonDefaultConstructibleFunctions::notDefaultConstructibleFunc]).AlwaysReturn(NotDefaultConstructible(1));
			NonDefaultConstructibleFunctions& i = mock.get();
			Assert::IsTrue(NotDefaultConstructible(1) == i.notDefaultConstructibleFunc());
			Assert::IsFalse(NotDefaultConstructible(2) == i.notDefaultConstructibleFunc());
		}

		TEST_METHOD(OverrideDefualtBehavior_AbstractType)
		{
			Mock<ReferenceFunctions> mock;
			ReferenceFunctions* pAbstractType{ nullptr };
			ReferenceFunctions& refAbstractType{ *pAbstractType };
			When(mock[&ReferenceFunctions::abstractTypeFunc]).Return(refAbstractType);
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
			Stub(mock[&SomeInterface::proc]);
			Stub(mock[&SomeInterface::func]);
			SomeInterface& i = mock.get();
			i.proc(1);
			i.proc(2);
			Assert::AreEqual(0, i.func(1));
			Assert::AreEqual(0, i.func(2));
		}

		TEST_METHOD(OverideDefualtBehaviorMatchAllInvocations)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::proc]).AlwaysThrow(std::string());
			When(mock[&SomeInterface::func]).AlwaysReturn(1);
			SomeInterface& i = mock.get();
			
 			Assert::ExpectException<std::string>([&](){i.proc(1); });
 			Assert::ExpectException<std::string>([&](){i.proc(2); });
			Assert::AreEqual(1, i.func(1));
			Assert::AreEqual(1, i.func(2));
		}

		TEST_METHOD(OverideDefualtBehaviorMatchAllInvocations_WithDo)
		{
			Mock<SomeInterface> mock;
			When(mock[&SomeInterface::proc]).AlwaysDo([](...){throw std::string(); });
			When(mock[&SomeInterface::func]).AlwaysDo([](...){return 1; });
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
			Stub(mock[&UnionFunctions::unionFunc]);
			UnionFunctions& i = mock.get();
			Assert::IsTrue(MyUnion() == i.unionFunc());
		}

		TEST_METHOD(ReturnByReference_ReturnReferenceToDefaultConstructedUnion)
		{
			Mock<UnionFunctions> mock;
			Stub(mock[&UnionFunctions::unionRefFunc]);
			Stub(mock[&UnionFunctions::unionRefFunc]);
			UnionFunctions& i = mock.get();
			Assert::IsTrue(MyUnion() == i.unionRefFunc());
		}

	};
}