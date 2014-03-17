/*
 * sequence_verification_tests.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: eran
 */
#include <string>

#include "tpunit++.hpp"
#include "fakeit.h"

using namespace fakeit;

struct SequenceVerification: tpunit::TestFixture {
	SequenceVerification() :
			tpunit::TestFixture(
			//
					TEST(SequenceVerification::verify_concatenated_sequence),
					TEST(SequenceVerification::verify_repeated_sequence), //
					TEST(SequenceVerification::verify_repeated_sequence_2), //
					TEST(SequenceVerification::verify_multi_sequences_in_order),
					TEST(SequenceVerification::should_throw_argument_exception_on_invalid_repetiotions_number)
					)//
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

} __SequenceVerification;
