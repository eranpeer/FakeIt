/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct InheritedFuncsTests : tpunit::TestFixture
{

    InheritedFuncsTests() :
        TestFixture(
            TEST(InheritedFuncsTests::mock_base_functions)
        )
    {
    }

    class BaseInterface
    {
    public:
        virtual ~BaseInterface() = default;

        virtual double nonOverloadedMethod() = 0;
        virtual double overloadedMethod() = 0;
        virtual double overloadedMethod() const = 0;
    };

    class Interface : public BaseInterface
    {
    public:
        ~Interface() override = default;
    };

    void mock_base_functions()
    {
        Mock<Interface> mock;

        When(Method(mock, nonOverloadedMethod)).Return(1.5);
        When(OverloadedMethod(mock, overloadedMethod, double())).Return(2.5);
        When(ConstOverloadedMethod(mock, overloadedMethod, double())).Return(3.5);

        Interface& interface = mock.get();
        const Interface& constInterface = mock.get();

        EXPECT_EQUAL(interface.nonOverloadedMethod(), 1.5);
        EXPECT_EQUAL(interface.overloadedMethod(), 2.5);
        EXPECT_EQUAL(constInterface.overloadedMethod(), 3.5);

        Verify(Method(mock, nonOverloadedMethod)).Exactly(1);
        Verify(OverloadedMethod(mock, overloadedMethod, double())).Exactly(1);
        Verify(ConstOverloadedMethod(mock, overloadedMethod, double())).Exactly(1);
    }

} __InheritedFuncsTests;
