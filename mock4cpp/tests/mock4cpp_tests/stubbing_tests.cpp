#include "tpunit++.hpp"
#include "mock4cpp.h"

struct BasicStubbing: tpunit::TestFixture {
	BasicStubbing() :
			tpunit::TestFixture( //
					TEST(BasicStubbing::ShouldThrow_UnmockedMethodException_WhenCallingANonStubbedMethod), //
					TEST(BasicStubbing::ShouldReturn_DefaultValue_WhenCallingAStubbedMethodWithNoSpecifiedBehaviour), //
					TEST(BasicStubbing::test_b)  //
							)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		int a = 0;
	};

	void ShouldThrow_UnmockedMethodException_WhenCallingANonStubbedMethod() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		ASSERT_THROW(i.func(1),UnmockedMethodException);
		ASSERT_THROW(i.proc(1),UnmockedMethodException);
	}


	void ShouldReturn_DefaultValue_WhenCallingAStubbedMethodWithNoSpecifiedBehaviour() {
			Mock<SomeInterface> mock;
			//mock[&SomeInterface::func] = [](...){return 0;};
//			mock.When(&SomeInterface::func).Return(0);
//			mock.When(&SomeInterface::func).Do([](...){return 0;});
//			mock.When(&SomeInterface::proc).Return();
//			mock.When(&SomeInterface::proc).Do([](...){return ;});
//			mock.When(&SomeInterface::proc).Throw(1);
//			mock[&SomeInterface::proc];
//			mock[&SomeInterface::proc] = [](...){return ;};
//			mock[&SomeInterface::func] = [](...){return 1;};

			When(mock[&SomeInterface::func]).Do([](...){return 1;});
//			int a;
			SomeInterface &i = mock.get();
			ASSERT_EQUAL(1,i.func(1));
//			ASSERT_THROW(i.proc(1),UnmockedMethodException);
	}

	void test_a() {
	}

	void test_b() {
	}

} __test_any;
