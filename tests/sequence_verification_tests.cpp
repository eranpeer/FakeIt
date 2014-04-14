/*
 * sequence_verification_tests.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: eran
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
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
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

		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(3)), fakeit::VerificationException);
	}

	void verify_repeated_sequence() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(mock[&SomeInterface::func] * 1).Exactly(6);
		Verify(mock[&SomeInterface::func] * 2).Exactly(3);
		Verify(mock[&SomeInterface::func] * 3).Exactly(2);
		Verify(mock[&SomeInterface::func] * 4).Exactly(1);
		Verify(mock[&SomeInterface::func] * 5).Exactly(1);
		Verify(mock[&SomeInterface::func] * 6).Exactly(1);

		Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(3)).Twice();
		Verify((mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(3)) * 2).Once();
		Verify(mock[&SomeInterface::func].Using(1) * 2).Never();

		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(1) * 2), fakeit::VerificationException);
	}

	void verify_repeated_sequence_2() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();

		i.func(1);
		i.func(2);
		i.func(3);
		i.func(1);
		i.func(2);
		i.func(3);

		Verify(1 * mock[&SomeInterface::func]).Exactly(6);
		Verify(2 * mock[&SomeInterface::func]).Exactly(3);
		Verify(3 * mock[&SomeInterface::func]).Exactly(2);
		Verify(4 * mock[&SomeInterface::func]).Exactly(1);
		Verify(5 * mock[&SomeInterface::func]).Exactly(1);
		Verify(6 * mock[&SomeInterface::func]).Exactly(1);

		Verify(2 * mock[&SomeInterface::func].Using(1)).Never();
		ASSERT_THROW(Verify(2 * mock[&SomeInterface::func].Using(1)), fakeit::VerificationException);
	}

	void verify_multi_sequences_in_order() {
		Mock<SomeInterface> mock;
		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
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
		.Exactly(1);

		Verify(  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Exactly(1);

		Verify(mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Exactly(1);

		Verify(mock[&SomeInterface::func],  //
				mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Exactly(2);

		Verify(mock[&SomeInterface::func],  //
				mock[&SomeInterface::func])  //
		.Exactly(3);

		Verify(mock[&SomeInterface::func])  //
		.Exactly(6);

		Verify(mock[&SomeInterface::func].Using(1) +  //
				mock[&SomeInterface::func].Using(2) + //
				mock[&SomeInterface::func].Using(3))  //
		.Exactly(2);

		Verify(mock[&SomeInterface::func].Using(1) + mock[&SomeInterface::func].Using(2))  //
		.Exactly(2);

		Verify(mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(3))  //
		.Exactly(2);

		Verify(mock[&SomeInterface::func].Using(1)).Exactly(2);

		Verify(mock[&SomeInterface::func].Using(2) + mock[&SomeInterface::func].Using(1)).Never();
	}

	void should_throw_argument_exception_on_invalid_repetiotions_number() {
		Mock<SomeInterface> mock;

		ASSERT_THROW(Verify(mock[&SomeInterface::func] * 0), std::invalid_argument);
		ASSERT_THROW(Verify(mock[&SomeInterface::func] * -1), std::invalid_argument);
		ASSERT_THROW(Verify(0 * mock[&SomeInterface::func]), std::invalid_argument);
		ASSERT_THROW(Verify(-1 * mock[&SomeInterface::func]), std::invalid_argument);
	}

	void use_only_mocks_that_are_involved_in_verifed_sequence_for_verification() {
		Mock<SomeInterface> mock1;
		Mock<SomeInterface> mock2;//

		Fake(mock1[&SomeInterface::func], mock2[&SomeInterface::func]);
		mock1.get().func(1);
		mock2.get().func(2);
		mock1.get().func(1);
		mock2.get().func(2);
		Verify(mock1[&SomeInterface::func]);
		Verify(mock1[&SomeInterface::func] * 2);

		Verify(mock2[&SomeInterface::func]);
		Verify(mock2[&SomeInterface::func] * 2);

		Using(mock1,mock2).Verify(mock1[&SomeInterface::func]);
		Using(mock1,mock2).Verify(mock2[&SomeInterface::func]);

		Using(mock1,mock2).Verify(mock1[&SomeInterface::func] + mock2[&SomeInterface::func]);
		Using(mock1,mock2).Verify((mock1[&SomeInterface::func] + mock2[&SomeInterface::func]) * 2);

		ASSERT_THROW(Using(mock1,mock2).Verify(mock1[&SomeInterface::func] * 2), fakeit::VerificationException);
		ASSERT_THROW(Using(mock1,mock2).Verify(mock2[&SomeInterface::func] * 2), fakeit::VerificationException);

		ASSERT_THROW(Using(mock1).Verify(mock2[&SomeInterface::func]), fakeit::VerificationException);
		ASSERT_THROW(Using(mock2).Verify(mock1[&SomeInterface::func]), fakeit::VerificationException);
	}

	void use_only_filters_that_are_involved_in_verifed_sequence_for_verification() {
		Mock<SomeInterface> mock1;

		Fake(mock1[&SomeInterface::func]);
		mock1.get().func(1);
		mock1.get().func(2);

		Using(mock1[&SomeInterface::func]).Verify(mock1[&SomeInterface::func] * 2).Exactly(1);
		Using(mock1[&SomeInterface::func]).Verify(mock1[&SomeInterface::func]).Exactly(2);
		Using(mock1[&SomeInterface::func].Using(1)).Verify(mock1[&SomeInterface::func]).Exactly(1);
		ASSERT_THROW(Using(mock1[&SomeInterface::func].Using(1)).Verify(mock1[&SomeInterface::func].Using(2)), fakeit::VerificationException);
		ASSERT_THROW(Using(mock1[&SomeInterface::func].Using(1)).Verify(mock1[&SomeInterface::func] * 2), fakeit::VerificationException);
	}

} __SequenceVerification;
