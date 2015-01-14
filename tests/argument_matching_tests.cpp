/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct ArgumentMatchingTests: tpunit::TestFixture {
	ArgumentMatchingTests()
			: tpunit::TestFixture(
			//
					TEST(ArgumentMatchingTests::test_eq_matcher),
					TEST(ArgumentMatchingTests::test_ge_matcher),
					TEST(ArgumentMatchingTests::test_lt_matcher),
					TEST(ArgumentMatchingTests::test_le_matcher),
					TEST(ArgumentMatchingTests::test_ne_matcher),
					TEST(ArgumentMatchingTests::test_gt_matcher),
					TEST(ArgumentMatchingTests::test_any_matcher),
					TEST(ArgumentMatchingTests::test_any_matcher2))//
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual int func2(int, std::string) = 0;
	};

	void test_eq_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(Eq(1))).Return(1);
		When(Method(mock, func).Using(Eq(2))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(2));

		Verify(Method(mock, func).Using(Eq(1))).Once();
		Verify(Method(mock, func).Using(Eq(2))).Once();
	}

	void test_gt_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(Gt(1))).AlwaysReturn(1);
		When(Method(mock, func).Using(Gt(2))).AlwaysReturn(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(2, i.func(3));

		Verify(Method(mock, func).Using(Gt(2))).Once();
		Verify(Method(mock, func).Using(Gt(3))).Never();
	}

	void test_ge_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func)).AlwaysReturn(-1);
		When(Method(mock, func).Using(Ge(1))).AlwaysReturn(1);
		When(Method(mock, func).Using(Ge(2))).AlwaysReturn(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(-1, i.func(0));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(2));

		Verify(Method(mock, func).Using(Ge(1))).Twice();
		Verify(Method(mock, func).Using(Ge(2))).Once();
	}

	void test_lt_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(Lt(1))).AlwaysReturn(1);
		When(Method(mock, func).Using(Lt(2))).AlwaysReturn(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_EQUAL(2, i.func(0));

		Verify(Method(mock, func).Using(Lt(2))).Twice();
		Verify(Method(mock, func).Using(Lt(1))).Once();
	}

	void test_le_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func)).AlwaysReturn(-1);
		When(Method(mock, func).Using(Le(1))).AlwaysReturn(1);
		When(Method(mock, func).Using(Le(2))).AlwaysReturn(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(2, i.func(2));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_EQUAL(-1, i.func(3));

		Verify(Method(mock, func).Using(Le(2))).Twice();
		Verify(Method(mock, func).Using(Le(1))).Once();
	}


	void test_ne_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func)).AlwaysReturn(-1);
		When(Method(mock, func).Using(Ne(1))).AlwaysReturn(1);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(-1, i.func(1));

		Verify(Method(mock, func).Using(Ne(1))).Once();
		Verify(Method(mock, func).Using(Ne(10))).Twice();
	}

	void test_any_matcher() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(Any)).AlwaysReturn(1);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(1, i.func(1));

		Verify(Method(mock, func).Using(Any)).Twice();
	}

	void test_any_matcher2() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(_)).AlwaysReturn(1);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(1, i.func(1));

		Verify(Method(mock, func).Using(_)).Twice();
	}

} __ArgumentMatching;

