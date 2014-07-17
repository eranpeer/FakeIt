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
					TEST(SequenceVerification::verify_concatenated_sequence), TEST(SequenceVerification::verify_repeated_sequence), //
					TEST(SequenceVerification::verify_repeated_sequence_2), //
					TEST(SequenceVerification::verify_multi_sequences_in_order), //
					TEST(SequenceVerification::use_only_mocks_that_are_involved_in_verifed_sequence_for_verification), //
					TEST(SequenceVerification::use_only_filters_that_are_involved_in_verifed_sequence_for_verification), //
					TEST(SequenceVerification::should_throw_argument_exception_on_invalid_repetiotions_number)) //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
	};

	void verify_concatenated_sequence() {
		Mock<SomeInterface> mock;
		Fake(CALL(mock,func), CALL(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(4);

		Verify(CALL(mock,func).Using(1) + CALL(mock,func).Using(2)).Once();
		Verify(CALL(mock,func).Using(2) + CALL(mock,func).Using(3)).AtLeastOnce();
		Verify(CALL(mock,func).Using(3) + CALL(mock,func).Using(4)).Once();
		Verify(CALL(mock,func) + CALL(mock,func)).Twice();
		Verify(CALL(mock,func).Using(1) + CALL(mock,func).Using(3)).Never();

		ASSERT_THROW(Verify(CALL(mock,func).Using(1) + CALL(mock,func).Using(3)), fakeit::VerificationException);
	}

	void verify_repeated_sequence() {
		Mock<SomeInterface> mock;
		Fake(CALL(mock,func), CALL(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(CALL(mock,func) * 1).Exactly(6);
		Verify(CALL(mock,func) * 2).Exactly(3);
		Verify(CALL(mock,func) * 3).Exactly(2);
		Verify(CALL(mock,func) * 4).Exactly(1);
		Verify(CALL(mock,func) * 5).Exactly(1);
		Verify(CALL(mock,func) * 6).Exactly(1);

		Verify(CALL(mock,func).Using(1) + CALL(mock,func).Using(2) + CALL(mock,func).Using(3)).Twice();
		Verify((CALL(mock,func).Using(1) + CALL(mock,func).Using(2) + CALL(mock,func).Using(3)) * 2).Once();
		Verify(CALL(mock,func).Using(1) * 2).Never();

		ASSERT_THROW(Verify(CALL(mock,func).Using(1) * 2), fakeit::VerificationException);
	}

	void verify_repeated_sequence_2() {
		Mock<SomeInterface> mock;
		Fake(CALL(mock,func), CALL(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(1 * CALL(mock,func)).Exactly(6);
		Verify(2 * CALL(mock,func)).Exactly(3);
		Verify(3 * CALL(mock,func)).Exactly(2);
		Verify(4 * CALL(mock,func)).Exactly(1);
		Verify(5 * CALL(mock,func)).Exactly(1);
		Verify(6 * CALL(mock,func)).Exactly(1);

		Verify(2 * CALL(mock,func).Using(1)).Never();
		ASSERT_THROW(Verify(2 * CALL(mock,func).Using(1)), fakeit::VerificationException);
	}

	void verify_multi_sequences_in_order() {
		Mock<SomeInterface> mock;
		Fake(CALL(mock,func), CALL(mock,proc));
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func))  //
		.Exactly(1);

		Verify(  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func))  //
		.Exactly(1);

		Verify(CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func))  //
		.Exactly(1);

		Verify(CALL(mock,func),  //
				CALL(mock,func),  //
				CALL(mock,func))  //
		.Exactly(2);

		Verify(CALL(mock,func),  //
				CALL(mock,func))  //
		.Exactly(3);

		Verify(CALL(mock,func))  //
		.Exactly(6);

		Verify(CALL(mock,func).Using(1) +  //
				CALL(mock,func).Using(2) + //
				CALL(mock,func).Using(3))  //
		.Exactly(2);

		Verify(CALL(mock,func).Using(1) + CALL(mock,func).Using(2))  //
		.Exactly(2);

		Verify(CALL(mock,func).Using(2) + CALL(mock,func).Using(3))  //
		.Exactly(2);

		Verify(CALL(mock,func).Using(1)).Exactly(2);

		Verify(CALL(mock,func).Using(2) + CALL(mock,func).Using(1)).Never();
	}

	void should_throw_argument_exception_on_invalid_repetiotions_number() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(CALL(mock,func) * 0), std::invalid_argument);
		ASSERT_THROW(Verify(CALL(mock,func) * -1), std::invalid_argument);
		ASSERT_THROW(Verify(0 * CALL(mock,func)), std::invalid_argument);
		ASSERT_THROW(Verify(-1 * CALL(mock,func)), std::invalid_argument);
	}

	void use_only_mocks_that_are_involved_in_verifed_sequence_for_verification() {
		Mock<SomeInterface> mock1;
		Mock<SomeInterface> mock2;//

		Fake(CALL(mock1,func), CALL(mock2,func));
		mock1.get().func(1);
		mock2.get().func(2);
		mock1.get().func(1);
		mock2.get().func(2);
		Verify(CALL(mock1,func));
		Verify(CALL(mock1,func) * 2);

		Verify(CALL(mock2,func));
		Verify(CALL(mock2,func) * 2);

		USING(mock1,mock2).VERIFY(CALL(mock1,func));
		USING(mock1,mock2).VERIFY(CALL(mock2,func));

		Using(mock1,mock2).VERIFY(CALL(mock1,func) + CALL(mock2,func));
		Using(mock1,mock2).VERIFY((CALL(mock1,func) + CALL(mock2,func)) * 2);

		ASSERT_THROW(Using(mock1,mock2).Verify(CALL(mock1,func) * 2), fakeit::VerificationException);
		ASSERT_THROW(Using(mock1,mock2).Verify(CALL(mock2,func) * 2), fakeit::VerificationException);

		ASSERT_THROW(Using(mock1).Verify(CALL(mock2,func)), fakeit::VerificationException);
		ASSERT_THROW(Using(mock2).Verify(CALL(mock1,func)), fakeit::VerificationException);
	}

	void use_only_filters_that_are_involved_in_verifed_sequence_for_verification() {
		Mock<SomeInterface> mock1;

		Fake(CALL(mock1,func));
		mock1.get().func(1);
		mock1.get().func(2);

		Using(CALL(mock1,func)).Verify(CALL(mock1,func) * 2).Exactly(1);
		Using(CALL(mock1,func)).Verify(CALL(mock1,func)).Exactly(2);
		Using(CALL(mock1,func).Using(1)).Verify(CALL(mock1,func)).Exactly(1);
		ASSERT_THROW(Using(CALL(mock1,func).Using(1)).Verify(CALL(mock1,func).Using(2)), fakeit::VerificationException);
		ASSERT_THROW(Using(CALL(mock1,func).Using(1)).Verify(CALL(mock1,func) * 2), fakeit::VerificationException);
	}

} __SequenceVerification;
