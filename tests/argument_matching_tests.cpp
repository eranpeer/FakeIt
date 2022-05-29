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

namespace {
	struct Base {
		virtual ~Base() = default;
		virtual int value() const { return 1; }
	};

	struct Derivated : public Base {
		int value() const override { return 2; }
	};

	bool operator==(const Base& lhs, const Base& rhs) {
		return lhs.value() == rhs.value();
	}

	struct WeirdType
	{
		template <typename T>
		WeirdType(T&&){}
	};
}

struct ArgumentMatchingTests: tpunit::TestFixture {
	ArgumentMatchingTests()
			: tpunit::TestFixture(
					//
					TEST(ArgumentMatchingTests::mixed_matchers),
					TEST(ArgumentMatchingTests::test_eq_matcher), TEST(ArgumentMatchingTests::test_ge_matcher),
					TEST(ArgumentMatchingTests::test_lt_matcher), TEST(ArgumentMatchingTests::test_le_matcher),
					TEST(ArgumentMatchingTests::test_ne_matcher), TEST(ArgumentMatchingTests::test_gt_matcher),
					TEST(ArgumentMatchingTests::test_str_eq_matcher), TEST(ArgumentMatchingTests::test_str_gt_matcher),
					TEST(ArgumentMatchingTests::test_str_ge_matcher), TEST(ArgumentMatchingTests::test_str_lt_matcher),
					TEST(ArgumentMatchingTests::test_str_le_matcher), TEST(ArgumentMatchingTests::test_str_ne_matcher),
					TEST(ArgumentMatchingTests::test_approx_eq_matcher),
					TEST(ArgumentMatchingTests::test_any_matcher), TEST(ArgumentMatchingTests::test_any_matcher2),
					TEST(ArgumentMatchingTests::test_any_matcher3), TEST(ArgumentMatchingTests::test_any_matcher_weird_constructor),
					TEST(ArgumentMatchingTests::pass_reference_by_value),
					TEST(ArgumentMatchingTests::format_Any), TEST(ArgumentMatchingTests::format_Eq),
					TEST(ArgumentMatchingTests::format_Gt), TEST(ArgumentMatchingTests::format_Ge),
					TEST(ArgumentMatchingTests::format_Lt), TEST(ArgumentMatchingTests::format_Le),
					TEST(ArgumentMatchingTests::format_Ne),
					TEST(ArgumentMatchingTests::format_StrEq), TEST(ArgumentMatchingTests::format_StrGt),
					TEST(ArgumentMatchingTests::format_StrGe), TEST(ArgumentMatchingTests::format_StrLt),
					TEST(ArgumentMatchingTests::format_StrLe), TEST(ArgumentMatchingTests::format_StrNe),
					TEST(ArgumentMatchingTests::format_ApproxEq),
					TEST(ArgumentMatchingTests::test_move_only_type), TEST(ArgumentMatchingTests::test_no_slicing)
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

	struct MoveOnlyType {
		const int i_;
		MoveOnlyType(int i) : i_{i} {}
		MoveOnlyType(const MoveOnlyType&) = delete;
		MoveOnlyType(MoveOnlyType&& o) : i_{o.i_} {};
		bool operator==(const MoveOnlyType& o) const { return i_ == o.i_; }
	};

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual int func2(int, std::string) = 0;
        virtual int func3(const int&) = 0;
        virtual int strfunc(const char*) = 0;
		virtual int funcMoveOnly(MoveOnlyType) = 0;
		virtual int funcSlicing(const Base&) = 0;
		virtual int funcDouble(double) = 0;
		virtual int funcWeirdType(int, WeirdType) = 0;
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

	void test_str_eq_matcher() {
		std::string second = "second";

		Mock<SomeInterface> mock;

		When(Method(mock, strfunc).Using(StrEq("first"))).Return(1);
		When(Method(mock, strfunc).Using(StrEq(second))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.strfunc("first"));
		ASSERT_EQUAL(2, i.strfunc("second"));

		Verify(Method(mock, strfunc).Using(StrEq("first"))).Once();
		Verify(Method(mock, strfunc).Using(StrEq("second"))).Once();
	}

	void test_str_gt_matcher() {
		std::string bb = "bb";

		Mock<SomeInterface> mock;

		When(Method(mock, strfunc).Using(StrGt("aa"))).Return(1);
		When(Method(mock, strfunc).Using(StrGt(bb))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.strfunc("ab"));
		ASSERT_EQUAL(2, i.strfunc("bc"));

		Verify(Method(mock, strfunc).Using(StrGt("aa"))).Twice();
		Verify(Method(mock, strfunc).Using(StrGt("bb"))).Once();
	}

	void test_str_ge_matcher() {
		std::string bb = "bb";

		Mock<SomeInterface> mock;

		When(Method(mock, strfunc).Using(StrGe("aa"))).Return(1);
		When(Method(mock, strfunc).Using(StrGe(bb))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.strfunc("ab"));
		ASSERT_EQUAL(2, i.strfunc("bb"));

		Verify(Method(mock, strfunc).Using(StrGe("aa"))).Twice();
		Verify(Method(mock, strfunc).Using(StrGe("bb"))).Once();
	}

	void test_str_lt_matcher() {
		std::string bb = "bb";

		Mock<SomeInterface> mock;

		When(Method(mock, strfunc).Using(StrLt("cc"))).Return(1);
		When(Method(mock, strfunc).Using(StrLt(bb))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.strfunc("cb"));
		ASSERT_EQUAL(2, i.strfunc("ba"));

		Verify(Method(mock, strfunc).Using(StrLt("cc"))).Twice();
		Verify(Method(mock, strfunc).Using(StrLt("bb"))).Once();
	}

	void test_str_le_matcher() {
		std::string bb = "bb";

		Mock<SomeInterface> mock;

		When(Method(mock, strfunc).Using(StrLe("cc"))).Return(1);
		When(Method(mock, strfunc).Using(StrLe(bb))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.strfunc("cc"));
		ASSERT_EQUAL(2, i.strfunc("ba"));

		Verify(Method(mock, strfunc).Using(StrLe("cc"))).Twice();
		Verify(Method(mock, strfunc).Using(StrLe("bb"))).Once();
	}

	void test_str_ne_matcher() {
		std::string second = "second";

		Mock<SomeInterface> mock;

		When(Method(mock, strfunc).Using(StrNe("first"))).Return(1);
		When(Method(mock, strfunc).Using(StrNe(second))).Return(2);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.strfunc("second"));
		ASSERT_EQUAL(2, i.strfunc("first"));

		Verify(Method(mock, strfunc).Using(StrNe("first"))).Once();
		Verify(Method(mock, strfunc).Using(StrNe("second"))).Once();
	}

	void test_approx_eq_matcher() {
		Mock<SomeInterface> mock;

		When(Method(mock, funcDouble).Using(ApproxEq(5, 0.2))).Return(1);
		When(Method(mock, funcDouble).Using(ApproxEq(7., 1))).Return(2);

		SomeInterface& i = mock.get();
		ASSERT_EQUAL(1, i.funcDouble(5.1));
		ASSERT_EQUAL(2, i.funcDouble(6.5));

		Verify(Method(mock, funcDouble).Using(ApproxEq(5, 0.2))).Once();
		Verify(Method(mock, funcDouble).Using(ApproxEq(7., 1))).Once();
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

	void test_any_matcher3() {

		Mock<SomeInterface> mock;

		When(Method(mock, func).Using(Any())).AlwaysReturn(1);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(2));
		ASSERT_EQUAL(1, i.func(1));

		Verify(Method(mock, func).Using(Any())).Twice();
	}

	void test_any_matcher_weird_constructor() {
		Mock<SomeInterface> mock;

		When(Method(mock, funcWeirdType).Using(1, _)).AlwaysReturn(1);
		When(Method(mock, funcWeirdType).Using(5, Any())).AlwaysReturn(5);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.funcWeirdType(1, WeirdType{10}));
		ASSERT_EQUAL(5, i.funcWeirdType(5, WeirdType{50}));

		Verify(Method(mock, funcWeirdType).Using(1, Any())).Once();
		Verify(Method(mock, funcWeirdType).Using(5, _)).Once();
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

	void format_StrEq() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, strfunc).Using(StrEq("first"))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.strfunc(first)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_StrGt() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, strfunc).Using(StrGt("first"))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.strfunc(>first)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_StrGe() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, strfunc).Using(StrGe("first"))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.strfunc(>=first)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_StrLt() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, strfunc).Using(StrLt("first"))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.strfunc(<first)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_StrLe() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, strfunc).Using(StrLe("first"))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.strfunc(<=first)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_StrNe() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, strfunc).Using(StrNe("first"))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.strfunc(!=first)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_ApproxEq() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, funcDouble).Using(ApproxEq(5, 0.1))).setFileInfo("test file", 1, "test method").Exactly(Once);
		} catch (SequenceVerificationException& e) {
			std::string expectedMsg{ formatLineNumner("test file", 1) };
			expectedMsg += ": Verification error\n";
			expectedMsg += "Expected pattern: mock.funcDouble(5+/-0.1)\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg { to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void test_move_only_type() {
		Mock<SomeInterface> mock;

		When(Method(mock, funcMoveOnly).Using(MoveOnlyType{10})).Return(1);
		When(Method(mock, funcMoveOnly).Using(Eq(MoveOnlyType{20}))).Return(2);

		SomeInterface& i = mock.get();
		ASSERT_EQUAL(1, i.funcMoveOnly(MoveOnlyType{10}));
		ASSERT_EQUAL(2, i.funcMoveOnly(MoveOnlyType{20}));

		Verify(Method(mock, funcMoveOnly).Using(MoveOnlyType{10})).Once();
		Verify(Method(mock, funcMoveOnly).Using(Eq(MoveOnlyType{20}))).Once();
	}

	void test_no_slicing() {
		Mock<SomeInterface> mock;

		When(Method(mock, funcSlicing).Using(Base{})).Return(1);
		When(Method(mock, funcSlicing).Using(Derivated{})).Return(2);

		SomeInterface& i = mock.get();
		ASSERT_EQUAL(1, i.funcSlicing(Base{}));
		ASSERT_EQUAL(2, i.funcSlicing(Derivated{}));

		//Not possible to verify because only references are stored in mock objects and they are dangling.
		//Verify(Method(mock, funcSlicing).Using(Base{})).Once();
		//Verify(Method(mock, funcSlicing).Using(Derivated{})).Once();
	}
} __ArgumentMatching;

