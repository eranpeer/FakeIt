/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct DefaultBehavioreTests : tpunit::TestFixture {
	DefaultBehavioreTests()
		: tpunit::TestFixture(
			//
			TEST(DefaultBehavioreTests::scalar_types_should_return_zero), //
			TEST(DefaultBehavioreTests::DefaultBeaviorOfVoidFunctionsIsToDoNothing), //
			TEST(DefaultBehavioreTests::ReturnByValue_ReturnDefaultConstructedObject), //
			TEST(DefaultBehavioreTests::ReturnByValue_ThrowExceptionIfNotDefaultConstructible), //
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
			TEST(DefaultBehavioreTests::ReturnByReference_ReturnReferenceToNullIfAbstract), //
#endif
			TEST(DefaultBehavioreTests::ReturnByReference_ReturnReferenceToDefaultConstructedObject), //
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
			TEST(DefaultBehavioreTests::ReturnByReference_ReturnReferenceToNullIfNotDefaultConstructible), //
#endif
			TEST(DefaultBehavioreTests::ReturnPtr_NullPtrIfPtrToAbstract),
			TEST(DefaultBehavioreTests::production_shared_ptr_mock_used_in_invocation)
			//TEST(DefaultBehavioreTests::should_survive_delete_of_mock_instance_by_user)
		) {
	}

	enum Color {
		RED = 1, GREEN = 2, BLUE = 3
	};

	struct ScalarFunctions {
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
		virtual ScalarFunctions * pScalarFuctionsfunc() = 0;
		virtual std::nullptr_t nullptrFunc() = 0;

		typedef bool (ScalarFunctions::*func)();
		virtual func pMemberFunc() = 0;
	};

	struct DefaultConstructibleFunctions {
		virtual std::string stringfunc() = 0;
	};

	struct NotDefaultConstructible {
		NotDefaultConstructible(int a)
			: _a(a) {
		}
		bool operator==(const NotDefaultConstructible &other) const {
			return _a == other._a;
		}
	private:
		int _a;
	};

	struct NonDefaultConstructibleFunctions {
		virtual NotDefaultConstructible notDefaultConstructibleFunc() = 0;
	};

	struct ReferenceFunctions {
		ReferenceFunctions() = default;
		virtual int& scalarFunc() = 0;
		virtual std::string& stringFunc() = 0;
		virtual NotDefaultConstructible& notDefaultConstructibleFunc() = 0;
		virtual ReferenceFunctions& abstractTypeFunc() = 0;
		virtual ReferenceFunctions* abstractTypeFunc2() = 0;

		bool operator==(const ReferenceFunctions &other) const {
			return this == &other;
		}
	};
	void scalar_types_should_return_zero() {
		Mock<ScalarFunctions> mock;

		Fake(Method(mock, boolFunc));
		Fake(Method(mock, charFunc));
		Fake(Method(mock, char16Func));
		Fake(Method(mock, char32Func));
		Fake(Method(mock, wcharFunc));
		Fake(Method(mock, shortFunc));
		Fake(Method(mock, intFunc));
		Fake(Method(mock, longFunc));
		Fake(Method(mock, longLongFunc));
		Fake(Method(mock, floatFunc));
		Fake(Method(mock, doubleFunc));
		Fake(Method(mock, longDoubleFunc));
		Fake(Method(mock, enumFunc));
		Fake(Method(mock, pIntFunc));
		Fake(Method(mock, pScalarFuctionsfunc));
		Fake(Method(mock, nullptrFunc));
		Fake(Method(mock, pMemberFunc));

		ScalarFunctions &i = mock.get();

		//Default behavior of a scalar function is to return 0/false/null

		ASSERT_EQUAL(false, i.boolFunc());
		ASSERT_EQUAL((char)0, i.charFunc());
		ASSERT_EQUAL((int)(char16_t)0, (int)i.char16Func());
		ASSERT_EQUAL((char32_t)0, i.char32Func());
		ASSERT_EQUAL((wchar_t)0, i.wcharFunc());
		ASSERT_EQUAL((short)0, i.shortFunc());
		ASSERT_EQUAL((int)0, i.intFunc());
		ASSERT_EQUAL((long)0, i.longFunc());
		ASSERT_EQUAL((long)0, (long)i.longLongFunc());
		ASSERT_EQUAL((float)0, i.floatFunc());
		ASSERT_EQUAL((double)0, i.doubleFunc());
		ASSERT_EQUAL((double)0, (double)i.longDoubleFunc());
		ASSERT_EQUAL(0, (int)i.enumFunc());
		ASSERT_EQUAL(nullptr, i.pIntFunc());
		ASSERT_EQUAL(nullptr, i.pScalarFuctionsfunc());
		ASSERT_EQUAL(nullptr, i.nullptrFunc());
		ASSERT_EQUAL(0, union_cast<int>(i.pMemberFunc()));
	}

	struct VoidFunctions {
		virtual void proc1() = 0;
		virtual void proc2(int a) = 0;
	};

	void DefaultBeaviorOfVoidFunctionsIsToDoNothing() {
		Mock<VoidFunctions> mock;
		Fake(Method(mock, proc1));
		Fake(Method(mock, proc2));
		VoidFunctions& i = mock.get();
		i.proc1();
		i.proc2(1);
	}

	void ReturnByValue_ReturnDefaultConstructedObject() {
		Mock<DefaultConstructibleFunctions> mock;
		Fake(Method(mock, stringfunc));
		DefaultConstructibleFunctions& i = mock.get();
		ASSERT_EQUAL(std::string(), i.stringfunc());
	}

	void ReturnByReference_ReturnReferenceToDefaultConstructedObject() {
		Mock<ReferenceFunctions> mock;
		Fake(Method(mock, scalarFunc));
		Fake(Method(mock, stringFunc));
		ReferenceFunctions& i = mock.get();
		ASSERT_EQUAL(0, i.scalarFunc());
		ASSERT_EQUAL(std::string(), i.stringFunc());
	}

	void ReturnByValue_ThrowExceptionIfNotDefaultConstructible() {
		Mock<NonDefaultConstructibleFunctions> mock;
		Fake(Method(mock, notDefaultConstructibleFunc));
		NonDefaultConstructibleFunctions& i = mock.get();
		try {
			i.notDefaultConstructibleFunc();
			FAIL()
				;
		}
		catch (fakeit::DefaultValueInstatiationException& e) {
			auto expected = std::string("Type ") + std::string(typeid(NotDefaultConstructible).name())
				+ std::string(" is not default constructible. Could not instantiate a default return value");
			std::string actual(e.what());
			ASSERT_EQUAL(expected, actual);
		}
	}

	void ReturnByReference_ReturnReferenceToNullIfNotDefaultConstructible() {
		Mock<ReferenceFunctions> mock;
		Fake(Method(mock, notDefaultConstructibleFunc));
		ReferenceFunctions& i = mock.get();
		ASSERT_EQUAL(nullptr, &i.notDefaultConstructibleFunc());
	}

	void ReturnByReference_ReturnReferenceToNullIfAbstract() {
		Mock<ReferenceFunctions> mock;
		Fake(Method(mock, abstractTypeFunc));
		ReferenceFunctions& i = mock.get();
		ASSERT_EQUAL(nullptr, &i.abstractTypeFunc());
	}

	void ReturnPtr_NullPtrIfPtrToAbstract() {
		Mock<ReferenceFunctions> mock;
		Fake(Method(mock, abstractTypeFunc2));
		ReferenceFunctions& i = mock.get();
		ASSERT_EQUAL(nullptr, i.abstractTypeFunc2());
	}

	class ISomeInterface3 {
	public:
		virtual ~ISomeInterface3() {
			int a = 0;
			a++;
		}

		virtual void methodWithSomeSharedPointer(std::shared_ptr<ISomeInterface3> listener) = 0;
	};

//	void production_shared_ptr_mock_used_in_invocation() {
//		std::shared_ptr<ISomeInterface3> pMockInstance;
//		Mock<ISomeInterface3> mock;
//		fakeit::Fake(Dtor(mock));
//		fakeit::Fake(Method(mock, methodWithSomeSharedPointer));
//
//		pMockInstance = mock.getShared();
//		pMockInstance->methodWithSomeSharedPointer(pMockInstance);
//
//		pMockInstance = nullptr;
//	}

	void production_shared_ptr_mock_used_in_invocation() {
		Mock<ISomeInterface3> mock;
		std::shared_ptr<ISomeInterface3> pMockInstance(&mock.get());
		fakeit::Fake(Dtor(mock));
		fakeit::Fake(Method(mock, methodWithSomeSharedPointer));
		pMockInstance->methodWithSomeSharedPointer(pMockInstance);
		pMockInstance = nullptr;
	}

} __DefaultBehaviore;
