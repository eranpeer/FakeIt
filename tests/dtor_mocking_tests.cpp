/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct DtorMocking: tpunit::TestFixture
{
    DtorMocking() :
		TestFixture(
		TEST(DtorMocking::mock_virtual_dtor) //
		)
	{
	}


    struct SomeInterface
    {
		virtual ~SomeInterface() {}
    };


	void mock_virtual_dtor() {
		Mock<SomeInterface> mock;
		When(Dtor(mock)).AlwaysReturn();
        SomeInterface * i = &(mock.get());
		delete i;
		delete i;
	}

} __DtorMocking;
