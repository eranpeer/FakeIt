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
            TEST(DtorMocking::mock_virtual_dtor_by_assignment)//
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

} __DtorMocking;
