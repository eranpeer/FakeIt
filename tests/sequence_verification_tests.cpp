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

struct SequenceVerification: tpunit::TestFixture {
	SequenceVerification() :
			tpunit::TestFixture(
					//
					TEST(SequenceVerification::verify_concatenated_sequence), //
					TEST(SequenceVerification::verify_with_user_types), //
					TEST(SequenceVerification::verify_repeated_sequence), //
					TEST(SequenceVerification::verify_repeated_sequence_2), //
					TEST(SequenceVerification::verify_multi_sequences_in_order), //
					TEST(SequenceVerification::use_only_mocks_that_are_involved_in_verifed_sequence_for_verification), //
					TEST(SequenceVerification::use_only_filters_that_are_involved_in_verifed_sequence_for_verification), //
					TEST(SequenceVerification::should_throw_argument_exception_on_invalid_repetiotions_number)) //
	{
	}

	class UserType {  };
	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		virtual void func2(UserType& a, int b) = 0;
	};

	void verify_with_user_types() {
		Mock<SomeInterface> mock;
		Fake(Method(mock, func2));
		SomeInterface &i = mock.get();
		UserType a;
		i.func2(a, 1);
		ASSERT_THROW(Verify(Method(mock, func2) * 2), fakeit::VerificationException);
	}

	void verify_concatenated_sequence() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(4);

		Verify(Method(mock,func).Using(1) + Method(mock,func).Using(2)).Once();
		Verify(Method(mock,func).Using(2) + Method(mock,func).Using(3)).AtLeastOnce();
		Verify(Method(mock,func).Using(3) + Method(mock,func).Using(4)).Once();
		Verify(Method(mock,func) + Method(mock,func)).Twice();
		Verify(Method(mock,func).Using(1) + Method(mock,func).Using(3)).Never();

		ASSERT_THROW(Verify(Method(mock,func).Using(1) + Method(mock,func).Using(3)), fakeit::VerificationException);
	}

	void verify_repeated_sequence() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(Method(mock,func) * 1).Exactly(6);
		Verify(Method(mock,func) * 2).Exactly(3);
		Verify(Method(mock,func) * 3).Exactly(2);
		Verify(Method(mock,func) * 4).Exactly(1);
		Verify(Method(mock,func) * 5).Exactly(1);
		Verify(Method(mock,func) * 6).Exactly(1);

		Verify(Method(mock,func).Using(1) + Method(mock,func).Using(2) + Method(mock,func).Using(3)).Twice();
		Verify((Method(mock,func).Using(1) + Method(mock,func).Using(2) + Method(mock,func).Using(3)) * 2).Once();
		Verify(Method(mock,func).Using(1) * 2).Never();

		ASSERT_THROW(Verify(Method(mock,func).Using(1) * 2), fakeit::VerificationException);
	}

	void verify_repeated_sequence_2() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(1 * Method(mock,func)).Exactly(6);
		Verify(2 * Method(mock,func)).Exactly(3);
		Verify(3 * Method(mock,func)).Exactly(2);
		Verify(4 * Method(mock,func)).Exactly(1);
		Verify(5 * Method(mock,func)).Exactly(1);
		Verify(6 * Method(mock,func)).Exactly(1);

		Verify(2 * Method(mock,func).Using(1)).Never();
		ASSERT_THROW(Verify(2 * Method(mock,func).Using(1)), fakeit::VerificationException);
	}

	void verify_multi_sequences_in_order() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func), Method(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		ASSERT_THROW(Verify(Method(mock,func), Method(mock,func)).Exactly(100), fakeit::VerificationException);

		Verify(  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func))  //
		.Exactly(1);

		ASSERT_THROW(Verify(Method(mock,func), Method(mock,func)).Exactly(100), fakeit::VerificationException);

		Verify(  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func))  //
		.Exactly(1);

		Verify(Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func))  //
		.Exactly(1);

		Verify(Method(mock,func),  //
				Method(mock,func),  //
				Method(mock,func))  //
		.Exactly(2);

		Verify(Method(mock,func),  //
				Method(mock,func))  //
		.Exactly(3);

		Verify(Method(mock,func))  //
		.Exactly(6);

		Verify(Method(mock,func).Using(1) +  //
				Method(mock,func).Using(2) + //
				Method(mock,func).Using(3))  //
		.Exactly(2);

		Verify(Method(mock,func).Using(1) + Method(mock,func).Using(2))  //
		.Exactly(2);

		Verify(Method(mock,func).Using(2) + Method(mock,func).Using(3))  //
		.Exactly(2);

		Verify(Method(mock,func).Using(1)).Exactly(2);

		Verify(Method(mock,func).Using(2) + Method(mock,func).Using(1)).Never();
	}

	void should_throw_argument_exception_on_invalid_repetiotions_number() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(Method(mock,func) * 0), std::invalid_argument);
		ASSERT_THROW(Verify(Method(mock,func) * -1), std::invalid_argument);
		ASSERT_THROW(Verify(0 * Method(mock,func)), std::invalid_argument);
		ASSERT_THROW(Verify(-1 * Method(mock,func)), std::invalid_argument);
	}

	void use_only_mocks_that_are_involved_in_verifed_sequence_for_verification() {
		Mock<SomeInterface> mock1;
		Mock<SomeInterface> mock2;//

		Fake(Method(mock1,func), Method(mock2,func));
		mock1.get().func(1);
		mock2.get().func(2);
		mock1.get().func(1);
		mock2.get().func(2);
		Verify(Method(mock1,func));
		Verify(Method(mock1,func) * 2);

		Verify(Method(mock2,func));
		Verify(Method(mock2,func) * 2);

		Using(mock1,mock2).Verify(Method(mock1,func));
		Using(mock1,mock2).Verify(Method(mock2,func));

		Using(mock1,mock2).Verify(Method(mock1,func) + Method(mock2,func));
		Using(mock1,mock2).Verify((Method(mock1,func) + Method(mock2,func)) * 2);

		ASSERT_THROW(Using(mock1,mock2).Verify(Method(mock1,func) * 2), fakeit::VerificationException);
		ASSERT_THROW(Using(mock1,mock2).Verify(Method(mock2,func) * 2), fakeit::VerificationException);

		ASSERT_THROW(Using(mock1).Verify(Method(mock2,func)), fakeit::VerificationException);
		ASSERT_THROW(Using(mock2).Verify(Method(mock1,func)), fakeit::VerificationException);
	}

	void use_only_filters_that_are_involved_in_verifed_sequence_for_verification() {
		Mock<SomeInterface> mock1;

		Fake(Method(mock1,func));
		mock1.get().func(1);
		mock1.get().func(2);

		Using(Method(mock1,func)).Verify(Method(mock1,func) * 2).Exactly(1);
		Using(Method(mock1,func)).Verify(Method(mock1,func)).Exactly(2);
		Using(Method(mock1,func).Using(1)).Verify(Method(mock1,func)).Exactly(1);
		ASSERT_THROW(Using(Method(mock1,func).Using(1)).Verify(Method(mock1,func).Using(2)), fakeit::VerificationException);
		ASSERT_THROW(Using(Method(mock1,func).Using(1)).Verify(Method(mock1,func) * 2), fakeit::VerificationException);
	}

} __SequenceVerification;
