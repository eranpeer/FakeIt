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
                    TEST(ArgumentMatchingTests::pass_reference_by_value),
					TEST(ArgumentMatchingTests::test_eq_matcher), TEST(ArgumentMatchingTests::test_ge_matcher),
					TEST(ArgumentMatchingTests::test_lt_matcher), TEST(ArgumentMatchingTests::test_le_matcher),
					TEST(ArgumentMatchingTests::test_ne_matcher), TEST(ArgumentMatchingTests::test_gt_matcher),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::test_any_matcher2),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Any),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Eq),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Gt),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Ge),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Lt),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Le),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::format_Ne),
                    TEST(ArgumentMatchingTests::mixed_matchers)
			) //
	{
	}

    std::string formatLineNumner(std::string file, int num){
#ifndef __GNUG__
        return file + std::string("(") + std::to_string(num) + std::string(")");
#else
        return file + std::string(":") + std::to_string(num);
#endif
    }

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual int func2(int, std::string) = 0;
        virtual int func3(const int&) = 0;
    };

	void mixed_matchers() {
		Mock<SomeInterface> mock;

		When(Method(mock, func2).Using(_, _)).Return(0);
		When(Method(mock, func2).Using(1, _)).Return(1);
		When(Method(mock, func2).Using(2, _)).Return(2);
		When(Method(mock, func2).Using(_, "3")).Return(3);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func2(10, "2"));
		ASSERT_EQUAL(1, i.func2(1, "2"));
		ASSERT_EQUAL(2, i.func2(2, "2"));
		ASSERT_EQUAL(3, i.func2(1, "3"));

        When(Method(mock, func2).Using(Eq(4), "4")).Return(4);
        When(Method(mock, func2).Using(Eq(5), Eq(std::string("5")))).Return(5);
		When(Method(mock, func2).Using(Eq(6), Eq<std::string>("6"))).Return(6);

        ASSERT_EQUAL(4, i.func2(4, "4"));
        ASSERT_EQUAL(5, i.func2(5, "5"));
        ASSERT_EQUAL(6, i.func2(6, "6"));
    }

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

		When(Method(mock, func).Using(Any<int>())).AlwaysReturn(1);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(1, i.func(1));

		Verify(Method(mock, func).Using(Any<int>())).Twice();
	}

	void test_any_matcher2() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(_)).AlwaysReturn(1);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(1, i.func(1));

		Verify(Method(mock, func).Using(_)).Twice();
	}

    void pass_reference_by_value() {
//        Mock<SomeInterface> mock;
//        When(Method(mock, func3).Using(1)).AlwaysReturn(1);
//        SomeInterface &i = mock.get();
//        {
//            int a = 1;
//            i.func3(a);
//            a = 2;
//        }
//        Verify(Method(mock, func3).Using(Eq(1)));
    }

    void format_Any() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(_)).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
            expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(Any)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Eq() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(Eq(1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
            expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Gt() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(Gt(1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
            expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(>1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Ge() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(Ge(1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(>=1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Lt() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(Lt(1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
            expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(<1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Le() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(Le(1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(<=1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Ne() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func).Using(Ne(1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) };
            expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.func(!=1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}
} __ArgumentMatching;

