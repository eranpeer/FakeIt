#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "mock4cpp.h"
#include "mockutils/PrintType.h"

struct A {};

template<> struct Formatter<A> {
	static std::string format(const A& val) {
		return {"a"};
	}
};

struct BasicVerification: tpunit::TestFixture {
	BasicVerification() :
			tpunit::TestFixture(
			//
					TEST(BasicVerification::verify_should_not_throw_exception_if_method_was_called), //
					TEST(BasicVerification::verify_should_throw_VerificationException_if_method_was_not_called), //
					TEST(BasicVerification::verify_should_throw_VerificationException_if_method_was_not_stubbed), //
					TEST(BasicVerification::verify_method_was_called_at_least_once), //
					TEST(BasicVerification::verify_method_was_called_exactly_once), //
					TEST(BasicVerification::verify_method_was_never_called), //
					TEST(BasicVerification::verify_method_was_called_exactly_x_times), //
					TEST(BasicVerification::should_throw_IllegalArgumentException_on_negative_times_argument), //
					TEST(BasicVerification::verify_with_filter), //
					TEST(BasicVerification::verify_concatenated_sequence), //
					TEST(BasicVerification::verify_repeated_sequence), //
					TEST(BasicVerification::verify_multi_sequences_in_order), TEST(BasicVerification::verify_no_other_invocations_for_mock), //
					TEST(BasicVerification::verify_no_other_invocations_for_method_filter) //
							)  //
	{
	}


	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		virtual void proc2(int, SomeInterface&) = 0;
		virtual void proc3(int, const A&) = 0;
	};

	void verify_should_throw_VerificationException_if_method_was_not_called() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		ASSERT_THROW(Verify(mock[&SomeInterface::func]), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]), mock4cpp::VerificationException);
		//std::cout<<PrintType<int>(1);
		//std::cout<<PrintType<decltype(&SomeInterface::proc2)>()<<'\n';
		//std::cout<<PrintType<SomeInterface>()<<'\n';
	}

	void verify_should_throw_VerificationException_if_method_was_not_stubbed() {
		Mock<SomeInterface> mock;
		ASSERT_THROW(Verify(mock[&SomeInterface::func]), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]), mock4cpp::VerificationException);
	}

	void verify_should_not_throw_exception_if_method_was_called() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		i.proc(1);
		Verify(mock[&SomeInterface::func]);
		Verify(mock[&SomeInterface::proc]);
	}

	void verify_method_was_called_at_least_once() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]), mock4cpp::VerificationException);

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
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

	void verify_method_was_called_exactly_once() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Once(), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Once(), mock4cpp::VerificationException);

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Once(), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Once(), mock4cpp::VerificationException);

		i.func(1);
		i.proc(1);

		Verify(mock[&SomeInterface::func]).Once();
		Verify(mock[&SomeInterface::proc]).Once();

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Once(), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Once(), mock4cpp::VerificationException);
	}

	void verify_method_was_never_called() {
		Mock<SomeInterface> mock;

		Verify(mock[&SomeInterface::func]).Never();
		Verify(mock[&SomeInterface::proc]).Never();

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		Verify(mock[&SomeInterface::func]).Never();
		Verify(mock[&SomeInterface::proc]).Never();

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Never(), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Never(), mock4cpp::VerificationException);
	}

	void verify_method_was_called_exactly_x_times() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Times(2), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Times(2), mock4cpp::VerificationException);

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Times(2), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Times(2), mock4cpp::VerificationException);

		i.func(1);
		i.func(1);
		i.proc(1);
		i.proc(1);

		Verify(mock[&SomeInterface::func]).Times(2);
		Verify(mock[&SomeInterface::proc]).Times(2);

		i.func(1);
		i.proc(1);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Times(2), mock4cpp::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Times(2), mock4cpp::VerificationException);
	}

	void should_throw_IllegalArgumentException_on_negative_times_argument() {
		Mock<SomeInterface> mock;
		ASSERT_THROW(Verify(mock[&SomeInterface::func]).Times(-1), std::invalid_argument);
		ASSERT_THROW(Verify(mock[&SomeInterface::proc]).Times(-1), std::invalid_argument);
	}

	void verify_with_filter() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		Verify(mock[&SomeInterface::func].Using(1));
		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(2)), mock4cpp::VerificationException);
	}

	void verify_concatenated_sequence() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(4);

		Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(2)).Once();
		Verify(mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(3)).AtLeastOnce();
		Verify(mock[&SomeInterface::func].Using(3) + mock[&SomeInterface::func].Using(4)).Once();
		Verify(mock[&SomeInterface::func] + mock[&SomeInterface::func]).Twice();
		Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(3)).Never();

		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(3)), mock4cpp::VerificationException);
	}

	void verify_repeated_sequence() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(mock[&SomeInterface::func] * 1).Times(6);
		Verify(mock[&SomeInterface::func] * 2).Times(3);
		Verify(mock[&SomeInterface::func] * 3).Times(2);
		Verify(mock[&SomeInterface::func] * 4).Times(1);
		Verify(mock[&SomeInterface::func] * 5).Times(1);
		Verify(mock[&SomeInterface::func] * 6).Times(1);

		Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(3)).Twice();
		Verify((mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(3)) * 2).Once();
		Verify(mock[&SomeInterface::func].Using(1) * 2).Never();

		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(1) * 2), mock4cpp::VerificationException);
	}

	void verify_multi_sequences_in_order() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Times(1);

		Verify(  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Times(1);

		Verify(  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Times(1);

		Verify(  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Times(2);

		Verify(  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Times(3);

		Verify(  //
				mock[&SomeInterface::func])  //
		.Times(6);

		Verify(  //
				mock[&SomeInterface::func].Using(1) +  //
						mock[&SomeInterface::func].Using(2) + //
						mock[&SomeInterface::func].Using(3))  //
		.Times(2);

		Verify(  //
				mock[&SomeInterface::func].Using(1) +  //
						mock[&SomeInterface::func].Using(2))  //
		.Times(2);

		Verify(  //
				mock[&SomeInterface::func].Using(2) +  //
						mock[&SomeInterface::func].Using(3))  //
		.Times(2);

		Verify(  //
				mock[&SomeInterface::func].Using(1))  //
		.Times(2);

		Verify(mock[&SomeInterface::func].Using(2) +  //
				mock[&SomeInterface::func].Using(1)).Never();
	}

	void verify_no_other_invocations_for_mock() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc3]);
		SomeInterface &i = mock.get();
		VerifyNoOtherInvocations(mock);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), mock4cpp::VerificationException);

		Verify(mock[&SomeInterface::func]).AtLeastOnce();
		VerifyNoOtherInvocations(mock);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), mock4cpp::VerificationException);

		Verify(mock[&SomeInterface::func] * 3);
		ASSERT_THROW(VerifyNoOtherInvocations(mock), mock4cpp::VerificationException);

		Verify(mock[&SomeInterface::func] * 4);
		VerifyNoOtherInvocations(mock);
	}

	void verify_no_other_invocations_for_method_filter() {
		std::tuple<int> a { 1 };
//		std::cout<<a;
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc3]);
		SomeInterface &i = mock.get();
		VerifyNoOtherInvocations(mock[&SomeInterface::func]);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::func],mock[&SomeInterface::proc3]), mock4cpp::VerificationException);

		Verify(mock[&SomeInterface::func]).AtLeastOnce();
		VerifyNoOtherInvocations(mock[&SomeInterface::func]);

		i.func(1);
		i.func(1);
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::func]), mock4cpp::VerificationException);

		Verify(mock[&SomeInterface::func] * 3);
		ASSERT_THROW(VerifyNoOtherInvocations(mock[&SomeInterface::func]), mock4cpp::VerificationException);

		Verify(mock[&SomeInterface::func] * 4);
		VerifyNoOtherInvocations(mock[&SomeInterface::func].Using(1));
	}

} __BasicVerification;

