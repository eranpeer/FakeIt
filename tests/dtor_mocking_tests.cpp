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

struct DtorMocking : tpunit::TestFixture
{
	DtorMocking() :
		TestFixture(
            TEST(DtorMocking::mock_virtual_dtor_no_mocks),
            TEST(DtorMocking::mock_virtual_dtor_with_fake),
		    TEST(DtorMocking::mock_virtual_dtor_with_when),
            TEST(DtorMocking::mock_virtual_dtor_by_assignment),
            TEST(DtorMocking::call_dtor_without_delete),
            TEST(DtorMocking::spy_dtor),
            TEST(DtorMocking::spy_without_verify_dtor),
			TEST(DtorMocking::production_takes_ownwership_with_uniqe_ptr),
			TEST(DtorMocking::production_takes_ownership_interface_with_more_methods),
			TEST(DtorMocking::should_fail_faking_when_no_virtual_dtor_exists)
		)
	{
	}


	struct SomeInterface
	{
		virtual ~SomeInterface() = default;
	};

	void mock_virtual_dtor_no_mocks() {
		Mock<SomeInterface> mock;
		SomeInterface* i = &(mock.get());
		delete i;
	}

	void mock_virtual_dtor_with_fake() {
		Mock<SomeInterface> mock;
		Fake(Dtor(mock));
		SomeInterface * i = &(mock.get());
		delete i;
		delete i; // second delete should not throw exception
        Verify(Dtor(mock)).Twice();
		ASSERT_THROW(Verify(Dtor(mock)).Once(), fakeit::VerificationException);
	}

	void mock_virtual_dtor_with_when() {
		int a = 0;
		Mock<SomeInterface> mock;
		When(Dtor(mock)).Return().Do([&](){a++; });
		SomeInterface * i = &(mock.get());
        delete i; // do nothing
        delete i; // a++
        ASSERT_EQUAL(1, a);
	}

    void mock_virtual_dtor_by_assignment() {
        int a = 0;
        Mock<SomeInterface> mock;
        Dtor(mock) = [&](){a++; };
        SomeInterface * i = &(mock.get());
        delete i; // do nothing
        delete i; // a++
        ASSERT_EQUAL(2, a);
    }

	void production_takes_ownwership_with_uniqe_ptr() {
		Mock<SomeInterface> mock;
		Fake(Dtor(mock));
		SomeInterface * i = &mock.get();
		std::unique_ptr<SomeInterface> ptr(i);
	}

    void call_dtor_without_delete() {
        Mock<SomeInterface> mock;
        Fake(Dtor(mock));
        SomeInterface * i = &mock.get();
        i->~SomeInterface();
        Verify(Dtor(mock)).Once();
		ASSERT_THROW(Verify(Dtor(mock)).Twice(), fakeit::VerificationException);
	}

    struct A {
		virtual ~A(){}
    };

	void spy_dtor() {
        A a;
        Mock<A> mock(a);
        Spy(Dtor(mock));
        A * i = &mock.get();
        delete i;
        delete i;
        Verify(Dtor(mock)).Twice();
		ASSERT_THROW(Verify(Dtor(mock)).Once(), fakeit::VerificationException);
    }

	void spy_without_verify_dtor() {
		A a;
		Mock<A> mock(a);
		SpyWithoutVerify(Dtor(mock));
		A * i = &mock.get();
		delete i;
		delete i;
		Verify(Dtor(mock)).Twice();
		ASSERT_THROW(Verify(Dtor(mock)).Once(), fakeit::VerificationException);
	}

	class SomeInterface2 {
	public:
		virtual void DoSomething() = 0;
		virtual ~SomeInterface2() = default;
	};

	void production_takes_ownership_interface_with_more_methods() {
		Mock<SomeInterface2> m;

		Fake(Method(m, DoSomething));
		Fake(Dtor(m));

		std::unique_ptr<SomeInterface2> ptr = std::unique_ptr<SomeInterface2>(&m.get());		
		ptr = nullptr;
	}

	struct NoVirtualDestructor {
		~NoVirtualDestructor() {}
		virtual void foo() = 0;
	};

	void should_fail_faking_when_no_virtual_dtor_exists() {
		Mock<NoVirtualDestructor> m;
		ASSERT_THROW(Fake(Dtor(m)), std::runtime_error);
	}

} __DtorMocking;
