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
                TEST(InheritedFuncsTests::mock_super_base_overloaded_functions),
                TEST(InheritedFuncsTests::mock_super_base_and_base_overloaded_functions),
                TEST(InheritedFuncsTests::mock_super_base_and_child_overloaded_functions)
        )
    {
    }

    class SuperBaseInterface
    {
    public:
        virtual ~SuperBaseInterface() = default;

        virtual double nonOverloadedMethod() = 0;
        virtual double overloadedMethod() = 0;
        virtual double overloadedMethod() const = 0;
        virtual double overloadedInBaseMethod() = 0;
        virtual double overloadedInChildMethod() = 0;
    };

    class BaseInterface : public SuperBaseInterface
    {
    public:
        ~BaseInterface() override = default;

        using SuperBaseInterface::overloadedInBaseMethod;
        virtual double overloadedInBaseMethod() const = 0;
    };

    class Interface : public BaseInterface
    {
    public:
        ~Interface() override = default;

        using BaseInterface::overloadedInChildMethod;
        virtual double overloadedInChildMethod() const = 0;
    };

    void mock_super_base_overloaded_functions()
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

    void mock_super_base_and_base_overloaded_functions()
    {
        Mock<Interface> mock;

        When(OverloadedMethod(mock, overloadedInBaseMethod, double())).Return(4.5);
        When(ConstOverloadedMethod(mock, overloadedInBaseMethod, double())).Return(5.5);

        Interface& interface = mock.get();
        const Interface& constInterface = mock.get();

        EXPECT_EQUAL(interface.overloadedInBaseMethod(), 4.5);
        EXPECT_EQUAL(constInterface.overloadedInBaseMethod(), 5.5);

        Verify(OverloadedMethod(mock, overloadedInBaseMethod, double())).Exactly(1);
        Verify(ConstOverloadedMethod(mock, overloadedInBaseMethod, double())).Exactly(1);
    }

    void mock_super_base_and_child_overloaded_functions()
    {
        Mock<Interface> mock;

        When(OverloadedMethod(mock, overloadedInChildMethod, double())).Return(6.5);
        When(ConstOverloadedMethod(mock, overloadedInChildMethod, double())).Return(7.5);

        Interface& interface = mock.get();
        const Interface& constInterface = mock.get();

        EXPECT_EQUAL(interface.overloadedInChildMethod(), 6.5);
        EXPECT_EQUAL(constInterface.overloadedInChildMethod(), 7.5);

        Verify(OverloadedMethod(mock, overloadedInChildMethod, double())).Exactly(1);
        Verify(ConstOverloadedMethod(mock, overloadedInChildMethod, double())).Exactly(1);
    }

} __InheritedFuncsTests;
