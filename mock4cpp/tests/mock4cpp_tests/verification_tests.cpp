#include "tpunit++.hpp"
#include "mock4cpp.h"
#include <string>

struct BasicVerification: tpunit::TestFixture {
	BasicVerification() :
			tpunit::TestFixture(
			//
					TEST(BasicVerification::verify_should_not_throw_exception_if_method_was_called), //
					TEST(BasicVerification::verify_should_throw_AssertionException_if_method_was_not_called), //
					TEST(BasicVerification::verify_with_filter)
							)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		int a = 0;
	};

	void verify_should_not_throw_exception_if_method_was_called() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		Verify(mock[&SomeInterface::func]);
	}

	void verify_should_throw_AssertionException_if_method_was_not_called() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		ASSERT_THROW(Verify(mock[&SomeInterface::func]), mock4cpp::AssertionException);
	}

	void verify_with_filter() {
		Mock<SomeInterface> mock;
		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);
		SomeInterface &i = mock.get();
		i.func(1);
		Verify(mock[&SomeInterface::func].Using(1));
		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(2)), mock4cpp::AssertionException);
	}


} __BasicVerification;
