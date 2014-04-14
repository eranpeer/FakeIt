/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"

struct A {
	int state;
	A() :
			state(0) {
	}

	bool operator==(const A& other) const {
		return other.state == state;
	}
};

namespace fakeit {
template<> struct Formatter<A> {
	static std::string format(const A& val) {
		return {"a"};
	}
};
}

using namespace fakeit;

struct BasicVerification: tpunit::TestFixture {
	BasicVerification() :
			tpunit::TestFixture(
			//
					TEST(BasicVerification::verify_should_not_throw_exception_if_method_was_called), //
					TEST(BasicVerification::verify_should_throw_VerificationException_if_method_was_not_called), //
					TEST(BasicVerification::verify_should_throw_VerificationException_if_method_was_not_stubbed), //
					TEST(BasicVerification::verify_method_was_called_at_least_once), //
					TEST(BasicVerification::verify_method_was_called_at_least_twice), //
					TEST(BasicVerification::verify_method_was_called_at_least_X_with_quantifier), //
					TEST(BasicVerification::verify_method_was_called_exactly_once), //
					TEST(BasicVerification::verify_method_was_never_called), //
					TEST(BasicVerification::verify_method_was_called_exactly_x_times), //
					TEST(BasicVerification::verify_method_was_called_exactly_x_times_with_quantifier), //
					TEST(BasicVerification::should_throw_IllegalArgumentException_on_negative_times_argument), //
					TEST(BasicVerification::verify_with_filter), //
					TEST(BasicVerification::verify_no_other_invocations_for_mock), //
					TEST(BasicVerification::verify_no_other_invocations_for_method_filter), //
					TEST(BasicVerification::use_same_filter_for_both_stubbing_and_verification), //
					TEST(BasicVerification::verify_after_paramter_was_changed__with_Matching), //
					TEST(BasicVerification::verify_after_paramter_was_changed_with_Using)) //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		virtual void proc2(const A&) = 0;
		virtual void proc3(const A*) = 0;
	};

	void verify_should_throw_VerificationException_if_method_was_not_called() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		ASSERT_THROW(Verify(mock[&SomeInterface::func]), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]), fakeit::VerificationException);
	}

	void verify_should_throw_VerificationException_if_method_was_not_stubbed() {
		Mock<SomeInterface> mock;
		ASSERT_THROW(Verify(mock[&SomeInterface::func]), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]), fakeit::VerificationException);
	}

	void verify_should_not_throw_exception_if_method_was_called() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(1);
		Verify(mock[&SomeInterface::func]);
		Verify(mock[&SomeInterface::proc]);
	}

	void verify_method_was_called_at_least_once() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]), fakeit::VerificationException);

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(2);

		Verify(mock[&SomeInterface::func]);
		Verify(mock[&SomeInterface::proc]);

		i.func(1);
		i.proc(2);

		Verify(mock[&SomeInterface::func]);
		Verify(mock[&SomeInterface::proc]);
	}

	void verify_method_was_called_at_least_twice() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).AtLeast(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).AtLeast(2), fakeit::VerificationException);

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(2);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).AtLeast(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).AtLeast(2), fakeit::VerificationException);

		i.func(1);
		i.proc(2);

		Verify(mock[&SomeInterface::func]).AtLeast(2);
		Verify(mock[&SomeInterface::proc]).AtLeast(2);

		i.func(1);
		i.proc(2);

		Verify(mock[&SomeInterface::func]).AtLeast(2);
		Verify(mock[&SomeInterface::proc]).AtLeast(2);
	}

	void verify_method_was_called_at_least_X_with_quantifier() {
		Mock<SomeInterface> mock;

#if defined (__GNUG__)
		ASSERT_THROW(Verify(mock[&SomeInterface::func]).AtLeast(2_Times), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).AtLeast(2_Times), fakeit::VerificationException);
#endif

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(2);

#if defined (__GNUG__)
		ASSERT_THROW(Verify(mock[&SomeInterface::func]).AtLeast(2_Times), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).AtLeast(2), fakeit::VerificationException);
#endif
		ASSERT_THROW(Verify(mock[&SomeInterface::func]).AtLeast(Times<2>()), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).AtLeast(Times<2>()), fakeit::VerificationException);
		i.func(1);
		i.proc(2);

#if defined (__GNUG__)
		Verify(mock[&SomeInterface::func]).AtLeast(2_Times);
		Verify(mock[&SomeInterface::proc]).AtLeast(2_Times);
#endif
		Verify(mock[&SomeInterface::func]).AtLeast(Times<2>());
		Verify(mock[&SomeInterface::proc]).AtLeast(Times<2>());
		i.func(1);
		i.proc(2);

#if defined (__GNUG__)
		Verify(mock[&SomeInterface::func]).AtLeast(2_Times);
		Verify(mock[&SomeInterface::proc]).AtLeast(2_Times);
#endif
		Verify(mock[&SomeInterface::func]).AtLeast(Times<2>());
		Verify(mock[&SomeInterface::proc]).AtLeast(Times<2>());
	}

	void verify_method_was_called_exactly_once() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Once(), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Once(), fakeit::VerificationException);

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Once(), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Once(), fakeit::VerificationException);

		i.func(1);
		i.proc(1);

		Verify(mock[&SomeInterface::func]).Once();
		Verify(mock[&SomeInterface::proc]).Once();

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Once(), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Once(), fakeit::VerificationException);
	}

	void verify_method_was_never_called() {
		Mock<SomeInterface> mock;

		Verify(mock[&SomeInterface::func]).Never();
		Verify(mock[&SomeInterface::proc]).Never();

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		Verify(mock[&SomeInterface::func]).Never();
		Verify(mock[&SomeInterface::proc]).Never();

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Never(), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Never(), fakeit::VerificationException);
	}

	void verify_method_was_called_exactly_x_times_with_quantifier() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);

#if defined (__GNUG__)
// Only supported by GCC

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(2_Time), std::invalid_argument);
		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(1_Time), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(1_Time), fakeit::VerificationException);

#endif

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(Times<1>()), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(Times<1>()), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(Once), fakeit::VerificationException);

		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(1);

#if defined (__GNUG__)
// Only supported by GCC

		Verify(mock[&SomeInterface::func]).Exactly(1_Time);
		Verify(mock[&SomeInterface::proc]).Exactly(1_Time);

#endif

		Verify(mock[&SomeInterface::func]).Exactly(Times<1>());
		Verify(mock[&SomeInterface::proc]).Exactly(Times<1>());
		Verify(mock[&SomeInterface::proc]).Exactly(Once);

		i.func(1);
		i.proc(1);

#if defined (__GNUG__)
// Only supported by GCC

		Verify(mock[&SomeInterface::func]).Exactly(2_Times);
		Verify(mock[&SomeInterface::proc]).Exactly(2_Times);

#endif

		Verify(mock[&SomeInterface::func]).Exactly(Times<2>());
		Verify(mock[&SomeInterface::proc]).Exactly(Times<2>());
	}

	void verify_method_was_called_exactly_x_times() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(2), fakeit::VerificationException);

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(2), fakeit::VerificationException);

		i.func(1);
		i.func(1);
		i.proc(1);
		i.proc(1);

		Verify(mock[&SomeInterface::func]).Exactly(2);
		Verify(mock[&SomeInterface::proc]).Exactly(2);

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(2), fakeit::VerificationException);
	}

	void should_throw_IllegalArgumentException_on_negative_times_argument() {
		Mock<SomeInterface> mock;
		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Exactly(-1), std::invalid_argument);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Exactly(-1), std::invalid_argument);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).AtLeast(-1), std::invalid_argument);
	}

	void verify_with_filter() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		Verify(mock[&SomeInterface::func].Using(1));
		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(2)), fakeit::VerificationException);
	}

	void verify_no_other_invocations_for_mock() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc2]);
		SomeInterface &i = mock.get();
		VerifyNoOtherInvocations(mock);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException);

		Verify(mock[&SomeInterface::func]).AtLeastOnce();
		VerifyNoOtherInvocations(mock);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException);

		Verify(mock[&SomeInterface::func] * 3);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException);

		Verify(mock[&SomeInterface::func] * 4);
		VerifyNoOtherInvocations(mock);
	}

	void verify_no_other_invocations_for_method_filter() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc2]);
		SomeInterface &i = mock.get();
		VerifyNoOtherInvocations(mock[&SomeInterface::func]);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::func], mock[&SomeInterface::proc2]), fakeit::VerificationException);

		Verify(mock[&SomeInterface::func]).AtLeastOnce();
		VerifyNoOtherInvocations(mock[&SomeInterface::func]);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::func]), fakeit::VerificationException);

		Verify(mock[&SomeInterface::func] * 3);
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::func]), fakeit::VerificationException);

		Verify(mock[&SomeInterface::func] * 4);
		VerifyNoOtherInvocations(mock[&SomeInterface::func].Using(1));
	}

	void use_same_filter_for_both_stubbing_and_verification() {
		Mock<SomeInterface> mock;
		auto any_func_invocation = mock[&SomeInterface::func];
		When(any_func_invocation).AlwaysReturn(1);
		SomeInterface &i = mock.get();
		i.func(1);
		i.func(1);
		Verify(2 * any_func_invocation);
	}

	void verify_after_paramter_was_changed__with_Matching() {
		Mock<SomeInterface> mock;
		auto any_A_with_state_1 = mock[&SomeInterface::proc2].Matching([](const A& a)->bool {
			return a.state == 1;
		});
		Fake(any_A_with_state_1);
		SomeInterface &i = mock.get();
		{
			A a;
			a.state = 1;
			i.proc2(a);
			i.proc2(a);
			a.state = 2;
		}

		Verify(2 * any_A_with_state_1);
	}

	void verify_after_paramter_was_changed_with_Using() {
		Mock<SomeInterface> mock;
		A a1;
		a1.state = 1;

		auto call_to_proc2_with_state_1 = mock[&SomeInterface::proc2].Using(a1);
		Fake(call_to_proc2_with_state_1);
		SomeInterface &i = mock.get();

		A a2;
		a2.state = 1;

		i.proc2(a2);
		i.proc2(a2);

		a2.state = 2;

		Verify(2 * call_to_proc2_with_state_1);
	}

} __BasicVerification;
