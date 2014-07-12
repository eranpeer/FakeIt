/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct SpyingTests: tpunit::TestFixture {

	SpyingTests() :
			tpunit::TestFixture( //
					TEST(SpyingTests::useOriginalClassMethodIfNotFaked), //
					TEST(SpyingTests::useOriginalClassMethodIfStubbed), //
					TEST(SpyingTests::returnToOrignalClassMethodsOnReset), //
					TEST(SpyingTests::dataMembersAreNotChangedOnReset), //
					TEST(SpyingTests::verifySpyiedMethodWasCalled), //
					TEST(SpyingTests::mockDestructordoesNotDeleteObject), //
					TEST(SpyingTests::canVerifyMethodAfterSpying)//
			) //
	{
	}

	class SomeClass {
	public:
		int a;
		virtual int func1(int arg) {
			return arg;
		}
		virtual int func2(int arg) {
			return arg;
		}
	};

	void useOriginalClassMethodIfNotFaked() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		Fake(Call(spy,func1)); // Override to return a default value.
		SomeClass& i = spy.get();
		ASSERT_EQUAL(0, i.func1(1)); // should return default int value (0)
		ASSERT_EQUAL(1, i.func2(1)); // should use original method
	}

	void useOriginalClassMethodIfStubbed() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		When(Call(spy,func1)).AlwaysReturn(10); // Override to return 10
		SomeClass& i = spy.get();
		ASSERT_EQUAL(10, i.func1(1)); // should return 10
		ASSERT_EQUAL(1, i.func2(1)); // func2 is not stubbed. should use original method
	}

	void returnToOrignalClassMethodsOnReset() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		When(Call(spy,func1)).AlwaysReturn(10); // Override to return 10
		When(Call(spy,func2)).AlwaysReturn(10); // Override to return 10
		spy.Reset();
		SomeClass& i = spy.get();
		ASSERT_EQUAL(1, i.func1(1));  // should use original method
		ASSERT_EQUAL(1, i.func2(1)); // should use original method
	}

	void dataMembersAreNotChangedOnReset() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);

		obj.a = 10;
		spy.Reset();
		SomeClass& i = spy.get();

		ASSERT_EQUAL(10, obj.a);
		ASSERT_EQUAL(10, i.a);
	}

	void verifySpyiedMethodWasCalled() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		Fake(Call(spy,func1),Call(spy,func2)); // Override to return a default value.
		SomeClass& i = spy.get();
		i.func1(1);
		Verify(Call(spy,func1));
		ASSERT_THROW(Verify(Call(spy,func2)),fakeit::VerificationException);
	}

	void mockDestructordoesNotDeleteObject() {
		struct SomeClass {
			bool deleted;
			SomeClass() :
					deleted(false) {
			}
			~SomeClass() {
				deleted = true;
			}
			virtual int func(int arg) {
				return arg;
			}
		};

		SomeClass obj;
		{
			Mock<SomeClass> spy(obj);
		}

		ASSERT_FALSE(obj.deleted);
	}

	void canVerifyMethodAfterSpying() {
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		Spy(Call(mock,func1));
		SomeClass& i = mock.get();
		ASSERT_EQUAL(1, i.func1(1));
		Verify(Call(mock,func1));
		Verify(Call(mock,func1)).Exactly(Once);
		VerifyNoOtherInvocations(mock);
	}

} __SpyingTests;
