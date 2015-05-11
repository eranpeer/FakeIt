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
            TEST(DtorMocking::mock_virtual_dtor_with_fake), //
		    TEST(DtorMocking::mock_virtual_dtor_with_when),
            TEST(DtorMocking::mock_virtual_dtor_by_assignment),
            TEST(DtorMocking::call_dtor_without_delete),
            TEST(DtorMocking::spy_dtor),
			TEST(DtorMocking::production_takes_ownwership_with_uniqe_ptr)//
		)
	{
	}


	struct SomeInterface
	{
		virtual ~SomeInterface() = default;
	};

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

} __DtorMocking;
