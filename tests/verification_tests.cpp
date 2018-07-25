/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct A {
	int state;
	A() :
			state(0) {
	}

	bool operator==(const A& other) const {
		return other.state == state;
	}
};

//namespace fakeit {
//template<> struct Formatter<A> {
//	static std::string format(const A&) {
//		return {"a"};
//	}
//};
//}

struct BasicVerification: tpunit::TestFixture {
	BasicVerification() :
			tpunit::TestFixture(
			//
                    TEST(BasicVerification::verificationProgressShouldBeConvertibleToBool), //
					TEST(BasicVerification::verifyWithUnverifiedFunctor), //
					TEST(BasicVerification::verifyWithUnverifiedFunctorWithUsing), //
					TEST(BasicVerification::verify_with_matcher), //
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
					TEST(BasicVerification::exception_while_verifying_no_more_interactions_should_cancel_verification), //
					TEST(BasicVerification::exception_while_verifying_should_cancel_verification), //
					TEST(BasicVerification::use_same_filter_for_both_stubbing_and_verification), //
					TEST(BasicVerification::verify_after_paramter_was_changed__with_Matching), //
					TEST(BasicVerification::verify_after_paramter_was_changed_with_argument_matcher), //
					TEST(BasicVerification::verify_no_invocations),
					TEST(BasicVerification::verify_after_paramter_was_changed_with_Using), //
					TEST(BasicVerification::verificationShouldTolerateNullString))
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		virtual void proc2(const A&) = 0;
		virtual void proc3(const A*) = 0;
		virtual void proc4(std::unique_ptr<int>) const = 0;
	};

	void verify_no_invocations()
	{
		Mock<SomeInterface> mock;
		Verify(Method(mock, func)).Never();
		Verify(Method(mock, proc)).Never();
		Verify(Method(mock, proc2)).Never();
		Verify(Method(mock, proc3)).Never();
		Verify(Method(mock, proc4)).Never();
	}

	void verify_with_matcher() {
		Mock<SomeInterface> mock;
		When(Method(mock, func).Matching([](...)->bool{return true;})).Return(0);
		SomeInterface &i = mock.get();
		i.func(1);
		ASSERT_THROW(Verify(Method(mock, func).Matching([](...)->bool{return true;}) * 2), fakeit::VerificationException);
	}

	void verify_should_throw_VerificationException_if_method_was_not_called() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		ASSERT_THROW(Verify(Method(mock,func)), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)), fakeit::VerificationException);
	}

	void verify_should_throw_VerificationException_if_method_was_not_stubbed() {
		Mock<SomeInterface> mock;
		ASSERT_THROW(Verify(Method(mock,func)), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)), fakeit::VerificationException);
	}

	void verify_should_not_throw_exception_if_method_was_called() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(1);
		Verify(Method(mock,func));
		Verify(Method(mock,proc));
	}

	void verify_method_was_called_at_least_once() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(Method(mock,func)), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)), fakeit::VerificationException);

		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(2);

		Verify(Method(mock,func));
		Verify(Method(mock,proc));

		i.func(1);
		i.proc(2);

		Verify(Method(mock,func));
		Verify(Method(mock,proc));
	}

	void verify_method_was_called_at_least_twice() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(Method(mock,func)).AtLeast(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).AtLeast(2), fakeit::VerificationException);

		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(2);

		ASSERT_THROW(Verify(Method(mock,func)).AtLeast(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).AtLeast(2), fakeit::VerificationException);

		i.func(1);
		i.proc(2);

		Verify(Method(mock,func)).AtLeast(2);
		Verify(Method(mock,proc)).AtLeast(2);

		i.func(1);
		i.proc(2);

		Verify(Method(mock,func)).AtLeast(2);
		Verify(Method(mock,proc)).AtLeast(2);
	}

	void verify_method_was_called_at_least_X_with_quantifier() {
		Mock<SomeInterface> mock;

#if defined (__GNUG__) || (_MSC_VER >= 1900)
		ASSERT_THROW(Verify(Method(mock,func)).AtLeast(2_Times), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).AtLeast(2_Times), fakeit::VerificationException);
#endif

		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(2);

#if defined (__GNUG__) || (_MSC_VER >= 1900)
		ASSERT_THROW(Verify(Method(mock,func)).AtLeast(2_Times), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).AtLeast(2), fakeit::VerificationException);
#endif
		ASSERT_THROW(Verify(Method(mock,func)).AtLeast(Times<2>()), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).AtLeast(Times<2>()), fakeit::VerificationException);
		i.func(1);
		i.proc(2);

#if defined (__GNUG__) || (_MSC_VER >= 1900)
		Verify(Method(mock,func)).AtLeast(2_Times);
		Verify(Method(mock,proc)).AtLeast(2_Times);
#endif
		Verify(Method(mock,func)).AtLeast(Times<2>());
		Verify(Method(mock,proc)).AtLeast(Times<2>());
		i.func(1);
		i.proc(2);

#if defined (__GNUG__) || (_MSC_VER >= 1900)
		Verify(Method(mock,func)).AtLeast(2_Times);
		Verify(Method(mock,proc)).AtLeast(2_Times);
#endif
		Verify(Method(mock,func)).AtLeast(Times<2>());
		Verify(Method(mock,proc)).AtLeast(Times<2>());
	}

	void verify_method_was_called_exactly_once() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(Method(mock,func)).Once(), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Once(), fakeit::VerificationException);

		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		ASSERT_THROW(Verify(Method(mock,func)).Once(), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Once(), fakeit::VerificationException);

		i.func(1);
		i.proc(1);

		Verify(Method(mock,func)).Once();
		Verify(Method(mock,proc)).Once();

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(Method(mock,func)).Once(), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Once(), fakeit::VerificationException);
	}

	void verify_method_was_never_called() {
		Mock<SomeInterface> mock;

		Verify(Method(mock,func)).Never();
		Verify(Method(mock,proc)).Never();

		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		Verify(Method(mock,func)).Never();
		Verify(Method(mock,proc)).Never();

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(Method(mock,func)).Never(), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Never(), fakeit::VerificationException);
	}

	void verify_method_was_called_exactly_x_times_with_quantifier() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));

#if defined (__GNUG__) || (_MSC_VER >= 1900)
// Only supported by GCC

		ASSERT_THROW(Verify(Method(mock,func)).Exactly(2_Time), std::invalid_argument);
		ASSERT_THROW(Verify(Method(mock,func)).Exactly(1_Time), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(1_Time), fakeit::VerificationException);

#endif

		ASSERT_THROW(Verify(Method(mock,func)).Exactly(Times<1>()), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(Times<1>()), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(Once), fakeit::VerificationException);

		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(1);

#if defined (__GNUG__) || (_MSC_VER >= 1900)
		Verify(Method(mock,func)).Exactly(1_Time);
		Verify(Method(mock,proc)).Exactly(1_Time);
#endif

		Verify(Method(mock,func)).Exactly(Times<1>());
		Verify(Method(mock,proc)).Exactly(Times<1>());
		Verify(Method(mock,proc)).Exactly(Once);

		i.func(1);
		i.proc(1);

#if defined (__GNUG__) || (_MSC_VER >= 1900)
		Verify(Method(mock,func)).Exactly(2_Times);
		Verify(Method(mock,proc)).Exactly(2_Times);
#endif

		Verify(Method(mock,func)).Exactly(Times<2>());
		Verify(Method(mock,proc)).Exactly(Times<2>());
	}

	void verify_method_was_called_exactly_x_times() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(Method(mock,func)).Exactly(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(2), fakeit::VerificationException);

		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		ASSERT_THROW(Verify(Method(mock,func)).Exactly(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(2), fakeit::VerificationException);

		i.func(1);
		i.func(1);
		i.proc(1);
		i.proc(1);

		Verify(Method(mock,func)).Exactly(2);
		Verify(Method(mock,proc)).Exactly(2);

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(Method(mock,func)).Exactly(2), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(2), fakeit::VerificationException);
	}

	void should_throw_IllegalArgumentException_on_negative_times_argument() {
		Mock<SomeInterface> mock;
		ASSERT_THROW(Verify(Method(mock,func)).Exactly(-1), std::invalid_argument);
		ASSERT_THROW(Verify(Method(mock,proc)).Exactly(-1), std::invalid_argument);
		ASSERT_THROW(Verify(Method(mock,proc)).AtLeast(-1), std::invalid_argument);
	}

	void verify_with_filter() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();
		i.func(1);
		Verify(Method(mock,func).Using(1));
		ASSERT_THROW(Verify(Method(mock,func).Using(2)), fakeit::VerificationException);
	}

	void verify_no_other_invocations_for_mock() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc2));
		SomeInterface &i = mock.get();
		VerifyNoOtherInvocations(mock);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException);

		Verify(Method(mock,func)).AtLeastOnce();
		VerifyNoOtherInvocations(mock);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException);

		Verify(Method(mock,func) * 3);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), fakeit::VerificationException);

		Verify(Method(mock,func) * 4);
		VerifyNoOtherInvocations(mock);
	}

	void verify_no_other_invocations_for_method_filter() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc2));
		SomeInterface &i = mock.get();
		VerifyNoOtherInvocations(Method(mock,func));

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(Method(mock,func), Method(mock,proc2)), fakeit::VerificationException);

		Verify(Method(mock,func)).AtLeastOnce();
		VerifyNoOtherInvocations(Method(mock,func));

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(Method(mock,func)), fakeit::VerificationException);

		Verify(Method(mock,func) * 3);
		ASSERT_THROW(VerifyNoOtherInvocations(Method(mock,func)), fakeit::VerificationException);

		Verify(Method(mock,func) * 4);
		VerifyNoOtherInvocations(Method(mock,func).Using(1));
	}

	void exception_while_verifying_no_more_interactions_should_cancel_verification() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func));
		mock.get().func(1);
		{
			try {
				const auto a = VerifyNoOtherInvocations(Method(mock,func));
				if (&a == &a) // use a to avoid unused variable compilation warning.
					throw std::runtime_error("some exception");
			} catch (std::runtime_error &) {
			}
		}
	}

	void exception_while_verifying_should_cancel_verification() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func));
		{
			try {
				const auto a = Verify(Method(mock,func));
				//const auto b = Verify(Method(mock, func)).Exactly(1);
				if (&a == &a) // use a to avoid unused variable compilation warning.
					throw std::runtime_error("some exception");
			} catch (std::runtime_error &) {
			}
		}
	}

	void use_same_filter_for_both_stubbing_and_verification() {
		Mock<SomeInterface> mock;
		auto any_func_invocation = Method(mock,func);
		auto any_proc_invocation = Method(mock,proc);

		When(any_func_invocation).AlwaysReturn(1);
		When(any_proc_invocation).AlwaysReturn();

		SomeInterface &i = mock.get();
		i.func(1);
		i.func(1);

		i.proc(1);
		i.proc(1);

		Verify(any_func_invocation);
		Verify(any_proc_invocation);

		Verify(2 * any_func_invocation);
		Verify(2 * any_proc_invocation);

		Verify(2 * any_func_invocation + 2 * any_proc_invocation);
	}

	void verify_after_paramter_was_changed__with_Matching() {
		Mock<SomeInterface> mock;
		auto any_A_with_state_1 = Method(mock,proc2).Matching([](const A& a)->bool {
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

		auto call_to_proc2_with_state_1 = Method(mock,proc2).Using(a1);
		Fake(call_to_proc2_with_state_1);
		SomeInterface &i = mock.get();

		A a2;
		a2.state = 1;

		i.proc2(a2);
		i.proc2(a2);

		a2.state = 2;

		Verify(2 * call_to_proc2_with_state_1);
	}

	void verify_after_paramter_was_changed_with_argument_matcher() {
		Mock<SomeInterface> mock;
		A a1;
		a1.state = 1;

		auto call_to_proc2_with_state_1 = Method(mock,proc2).Using(Eq(a1));
		Fake(call_to_proc2_with_state_1);
		SomeInterface &i = mock.get();

		A a2;
		a2.state = 1;

		i.proc2(a2);
		i.proc2(a2);

		a2.state = 2;

		Verify(2 * call_to_proc2_with_state_1);
	}


	void verifyWithUnverifiedFunctor(){

        struct AnInterface {
            virtual int func(int) = 0;
        };
        
        Mock<AnInterface> mock;
		When(Method(mock, func)).AlwaysReturn(0);

		AnInterface& obj = mock.get();

		obj.func(1);
        Unverified.Verify(Method(mock, func)).Once();
        ASSERT_THROW(Unverified.Verify(Method(mock, func)).Once(), fakeit::VerificationException);

		obj.func(1);
        obj.func(1);
        Verify(Method(mock, func) * 3);
        ASSERT_THROW(Unverified.Verify(Method(mock, func)), fakeit::VerificationException);

        obj.func(1);
        obj.func(1);
        Verify(Method(mock, func));
        ASSERT_THROW(Unverified.Verify(Method(mock, func)), fakeit::VerificationException);
    }

	void verifyWithUnverifiedFunctorWithUsing() {

		struct AnInterface {
			virtual int func(int) = 0;
		};

		Mock<AnInterface> mock;
		When(Method(mock, func)).AlwaysReturn(0);

		AnInterface &obj = mock.get();

		obj.func(1);
		Using(Unverified(mock)).Verify(Method(mock, func)).Once();
		ASSERT_THROW(Using(Unverified(mock)).Verify(Method(mock, func)).Once(), fakeit::VerificationException);
	}

	void verificationProgressShouldBeConvertibleToBool(){

		struct AnInterface {
			virtual int func(int) = 0;
		};

		Mock<AnInterface> mock;
		When(Method(mock, func)).AlwaysReturn(0);

		AnInterface &obj = mock.get();
		obj.func(1);

        ASSERT_FALSE(VerifyNoOtherInvocations(Method(mock, func)));
        ASSERT_TRUE(Verify(Method(mock, func)));
		ASSERT_TRUE(Verify(Method(mock, func).Using(1)));
        ASSERT_FALSE(Verify(Method(mock, func).Using(2)));
        ASSERT_TRUE(Verify(Method(mock, func).Using(1)).AtLeast(1));
        ASSERT_TRUE(Verify(Method(mock, func).Using(1)).AtLeastOnce());
        ASSERT_TRUE(Verify(Method(mock, func).Using(1)).Exactly(1));
        ASSERT_TRUE(Verify(Method(mock, func).Using(1)).Once());
        ASSERT_FALSE(Verify(Method(mock, func).Using(1)).Twice());
        ASSERT_FALSE(Verify(Method(mock, func).Using(1)).Never());
		ASSERT_TRUE(VerifyNoOtherInvocations(Method(mock, func)));


		ASSERT_FALSE(!Verify(Method(mock, func)));
		ASSERT_FALSE(!Verify(Method(mock, func).Using(1)));
		ASSERT_TRUE(!Verify(Method(mock, func).Using(2)));
		ASSERT_FALSE(!Verify(Method(mock, func).Using(1)).AtLeast(1));
		ASSERT_FALSE(!Verify(Method(mock, func).Using(1)).AtLeastOnce());
		ASSERT_FALSE(!Verify(Method(mock, func).Using(1)).Exactly(1));
		ASSERT_FALSE(!Verify(Method(mock, func).Using(1)).Once());
		ASSERT_TRUE(!Verify(Method(mock, func).Using(1)).Twice());
		ASSERT_TRUE(!Verify(Method(mock, func).Using(1)).Never());
		ASSERT_FALSE(!VerifyNoOtherInvocations(Method(mock, func)));
    }

	void verificationShouldTolerateNullString(){
		struct RefEater {
			virtual int eatChar(char*) = 0;
			virtual int eatConstChar(const char*) = 0;
		};

		Mock<RefEater> mock;
		When( Method( mock, eatChar ) ).AlwaysReturn( 0 );
		When( Method( mock, eatConstChar ) ).AlwaysReturn( 0 );

		RefEater& obj = mock.get();
		obj.eatConstChar( nullptr );
		obj.eatConstChar( "string" );
		obj.eatChar( nullptr );
		char str[] = { 'a', '\0' };
		obj.eatChar( str );

		Verify(Method(mock,eatChar)).Exactly(2);
		Verify(Method(mock,eatConstChar)).Exactly(2);
		ASSERT_THROW(Verify(Method(mock, eatChar)).Exactly(3), fakeit::VerificationException);
		ASSERT_THROW(Verify(Method(mock, eatConstChar)).Exactly(3), fakeit::VerificationException);
	}

} __BasicVerification;
