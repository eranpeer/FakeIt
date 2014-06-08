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
			TEST(SpyingTests::returnToOrignalClassMethodsOnReset) //
			) //
	{
	}

	struct SomeClass {
		virtual int func(int arg) {
			return arg;
		}
		virtual int func2(int arg) {
			return arg;
		}
	};

	void useOriginalClassMethodIfNotFaked() {
		SomeClass obj;
		Mock<SomeClass> s(obj);
		Fake(s[&SomeClass::func]); // Override to return a default value.
		SomeClass& i = s.get();
		ASSERT_EQUAL(0, i.func(1)); // should return default int value (0) 
		ASSERT_EQUAL(1, i.func2(1)); // should use original method
	}

	void useOriginalClassMethodIfStubbed() {
		SomeClass obj;
		Mock<SomeClass> s(obj);
		When(s[&SomeClass::func]).AlwaysReturn(10); // Override to return 10
		SomeClass& i = s.get();
		ASSERT_EQUAL(10, i.func(1)); // should return default int value (0) 
		ASSERT_EQUAL(1, i.func2(1)); // func2 is not stubbed. should use original method
	}

	void returnToOrignalClassMethodsOnReset() {
		SomeClass obj;
		Mock<SomeClass> s(obj);
		When(s[&SomeClass::func]).AlwaysReturn(10); // Override to return 10
		When(s[&SomeClass::func2]).AlwaysReturn(10); // Override to return 10
		s.Reset();
		SomeClass& i = s.get();
		ASSERT_EQUAL(1, i.func(1));  // should use original method
		ASSERT_EQUAL(1, i.func2(1)); // should use original method
	}

} __SpyingTests;

