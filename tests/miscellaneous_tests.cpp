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
#include <fakeit/Mock.hpp>
#include <fakeit/ActualInvocation.hpp>
#include <fakeit/api_functors.hpp>

using namespace fakeit;

struct Miscellaneous : tpunit::TestFixture
{
    Miscellaneous() :
        TestFixture(
        TEST(Miscellaneous::pass_mock_by_ref), //
        TEST(Miscellaneous::can_mock_class_without_default_constructor), //
        TEST(Miscellaneous::can_mock_class_with_protected_constructor), //
        TEST(Miscellaneous::mock_virtual_methods_of_base_class), //
        TEST(Miscellaneous::create_and_delete_fakit_instatnce), //
        TEST(Miscellaneous::testStubFuncWithRightValueParameter),
        TEST(Miscellaneous::testStubProcWithRightValueParameter)
        )
    {
    }

    struct SomeStruct
    {
        SomeStruct(int)
        {
        }

        virtual void foo() = 0;
    };

    void can_mock_class_without_default_constructor()
    {
        Mock<SomeStruct> mock;
        Fake(Method(mock, foo));
    }

    void can_mock_class_with_protected_constructor()
    {
        struct SomeClass
        {
            virtual void foo() = 0;
        protected:
            SomeClass(int)
            {
            }
        };
        Mock<SomeClass> mock;
        Fake(Method(mock, foo));
    }

    void create_and_delete_fakit_instatnce()
    {
        class MyFakeit :public DefaultFakeit {
            fakeit::EventHandler &accessTestingFrameworkAdapter() override {
                throw "not implemented";
            }
        };
        {
            StandaloneFakeit s;
            MyFakeit f;
        }
    }

    struct Change
    {
        virtual void change(uint8_t r, uint8_t g, uint8_t b) = 0;
    };

    void assertChanged(Mock<Change>& mock, uint8_t v1, uint8_t v2, uint8_t v3)
    {
        Verify(Method(mock, change).Using(v1, v2, v3));
    }

    void pass_mock_by_ref()
    {
        Mock<Change> mock;
        Change* change = &mock.get();
        When(Method(mock, change)).AlwaysReturn();
        change->change(1, 2, 3);
        assertChanged(mock, 1, 2, 3);
    }

    struct A
    {
        //~A() = 0;
        virtual int a1() = 0;
        virtual int a2() = 0;
    };

    struct B : public A
    {
        virtual ~B() {}
        virtual int b1() = 0;
        virtual int b2() = 0;
    };

    void mock_virtual_methods_of_base_class() {
        Mock<B> mock;
        When(Method(mock, b1)).Return(1);
        When(Method(mock, b2)).Return(2);
        When(Method(mock, a1)).Return(3);
        When(Method(mock, a2)).Return(4);

        ASSERT_EQUAL(1, mock().b1());
        ASSERT_EQUAL(2, mock().b2());
        ASSERT_EQUAL(3, mock().a1());
        ASSERT_EQUAL(4, mock().a2());
    }


    void testStubFuncWithRightValueParameter() {

        struct foo {
            virtual int bar(int &&) = 0;
        };

        Mock<foo> foo_mock;
        When(Method(foo_mock, bar)).AlwaysReturn(100);
        When(Method(foo_mock, bar).Using(1)).AlwaysReturn(1);
        When(Method(foo_mock, bar).Using(2)).AlwaysDo([](int &){return 2; });
        When(Method(foo_mock, bar).Using(3)).Do([](int){return 3; });
        Method(foo_mock, bar).Using(4) = 4;

        ASSERT_EQUAL(100, foo_mock.get().bar(5));
        ASSERT_EQUAL(1, foo_mock.get().bar(1));
        ASSERT_EQUAL(2, foo_mock.get().bar(2));
        ASSERT_EQUAL(3, foo_mock.get().bar(3));
        ASSERT_EQUAL(4, foo_mock.get().bar(4));
    }

    void testStubProcWithRightValueParameter() {

        struct foo {
            virtual void bar(int &&) = 0;
        };

        int rv3 = 0;
        int rv4 = 0;
        int rv5 = 0;

        Mock<foo> foo_mock;
        When(Method(foo_mock, bar).Using(1)).Return();
        When(Method(foo_mock, bar)).AlwaysReturn();
        When(Method(foo_mock, bar).Using(3)).AlwaysDo([&](int &){rv3 = 3; });
        When(Method(foo_mock, bar).Using(4)).Do([&](int &){rv4 = 4; });
        Method(foo_mock, bar).Using(5) = [&](int &){rv5 = 5; };

        foo_mock.get().bar(1);
        foo_mock.get().bar(2);
        foo_mock.get().bar(3);
        ASSERT_EQUAL(3, rv3);
        foo_mock.get().bar(4);
        ASSERT_EQUAL(4, rv4);
        foo_mock.get().bar(5);
        ASSERT_EQUAL(5, rv5);
    }


    template <int discriminator>
    struct DummyType {
        int value;
    };

    template <typename T>
    class ClassToMock {
    public:
        virtual unsigned int method() {
            return 0;
        }
    };

    template <typename T>
    class MockProxyClass {
        Mock<ClassToMock<T> > mock;

    public:
        MockProxyClass(unsigned int value) {
            When(Method(mock, method)).AlwaysReturn(value);
        }

        Mock<ClassToMock<T> >& getMock() {
            return mock;
        }
    };

    void testRemoveAmbiguityInDependentName()
    {
        // this is a compilation test. the following code should compile!!
        MockProxyClass<DummyType<2> > proxy(3);
    }

} __Miscellaneous;


struct RValueTypesTests : tpunit::TestFixture {

    class AbstractType {
    public:
        virtual void foo() = 0;
    };

    class ConcreteType : public AbstractType {
    public:
        int state;
        ConcreteType() :
            state(10) {
        }
        virtual void foo() override {
        }

        bool operator==(const ConcreteType& other) {
            return (other.state == this->state);
        }
    };

    using CompositeType = std::pair < int, int > ;

    struct RValueInterface {
        virtual int intRValueArg(int&&) = 0;
        virtual int compositeRValueArg(CompositeType&&) = 0;
        virtual int overloadedMethod(CompositeType&&) = 0;
        virtual CompositeType overloadedMethod(int&&) = 0;
        virtual RValueInterface& operator=(RValueInterface&&) = 0;
        virtual RValueInterface& operator=(const RValueInterface&) = 0;
    };

    RValueTypesTests() :
        tpunit::TestFixture(
        //
        TEST(RValueTypesTests::explicitStubbingDefaultReturnValues),
        TEST(RValueTypesTests::explicitStubbingReturnValues_with_AlwaysReturn),
        TEST(RValueTypesTests::explicitStubbingReturnValues_by_assignment),
        TEST(RValueTypesTests::explicitStubbingReturnValues)
        //
        ) {
    }

    void explicitStubbingDefaultReturnValues() {
        Mock<RValueInterface> mock;     //
        When(Method(mock, intRValueArg)).Return(); //
        When(Method(mock, compositeRValueArg)).Return(); //
        When(OverloadedMethod(mock, overloadedMethod, int(CompositeType&&))).Return(); //
        When(OverloadedMethod(mock, overloadedMethod, CompositeType(int&&))).Return(); //

        RValueInterface & i = mock.get();

        ASSERT_EQUAL(0, i.intRValueArg(0));
        ASSERT_EQUAL(0, i.compositeRValueArg(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(0, i.overloadedMethod(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(std::make_pair(0, 0), i.overloadedMethod(0));
    }

    void explicitStubbingReturnValues() {
        Mock<RValueInterface> mock;     //
        When(Method(mock, intRValueArg)).Return(1); //
        When(Method(mock, compositeRValueArg)).Return(2); //
        When(OverloadedMethod(mock, overloadedMethod, int(CompositeType&&))).Return(3); //
        
        When(OverloadedMethod(mock, overloadedMethod, CompositeType(int&&))).Return(std::make_pair(4, 5)); //
        When(OverloadedMethod(mock, operator=, RValueInterface&(RValueInterface&&))).Return(mock.get()); //
        When(OverloadedMethod(mock, operator=, RValueInterface&(const RValueInterface&))).Return(mock.get()); //

        RValueInterface & i = mock.get();

        ASSERT_EQUAL(1, i.intRValueArg(0));
        ASSERT_EQUAL(2, i.compositeRValueArg(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(3, i.overloadedMethod(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(std::make_pair(4, 5), i.overloadedMethod(0));
        ASSERT_EQUAL(&i, &i.operator=(i));
        ASSERT_EQUAL(&i, &i.operator=(std::move(i)));
    }

    void explicitStubbingReturnValues_with_AlwaysReturn() {
        Mock<RValueInterface> mock;     //
        When(Method(mock, intRValueArg)).AlwaysReturn(1); //
        When(Method(mock, compositeRValueArg)).AlwaysReturn(2); //
        When(OverloadedMethod(mock, overloadedMethod, int(CompositeType&&))).AlwaysReturn(3); //
        When(OverloadedMethod(mock, overloadedMethod, CompositeType(int&&))).AlwaysReturn(std::make_pair(4, 5)); //
        When(OverloadedMethod(mock, operator=, RValueInterface&(RValueInterface&&))).AlwaysReturn(mock.get()); //
        When(OverloadedMethod(mock, operator=, RValueInterface&(const RValueInterface&))).AlwaysReturn(mock.get()); //

        RValueInterface & i = mock.get();

        ASSERT_EQUAL(1, i.intRValueArg(0));
        ASSERT_EQUAL(2, i.compositeRValueArg(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(3, i.overloadedMethod(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(std::make_pair(4, 5), i.overloadedMethod(0));
        ASSERT_EQUAL(&i, &i.operator=(i));
        ASSERT_EQUAL(&i, &i.operator=(std::move(i)));
    }

    void explicitStubbingReturnValues_by_assignment() {
        Mock<RValueInterface> mock;     //
        Method(mock, intRValueArg) = 1; //
        Method(mock, compositeRValueArg) = 2; //
        OverloadedMethod(mock, overloadedMethod, int(CompositeType&&)) = 3; //
        OverloadedMethod(mock, overloadedMethod, CompositeType(int&&)) = std::make_pair(4, 5); //
        OverloadedMethod(mock, operator=, RValueInterface&(RValueInterface&&)) = mock.get(); //
        OverloadedMethod(mock, operator=, RValueInterface&(const RValueInterface&)) = mock.get(); //

        RValueInterface & i = mock.get();

        ASSERT_EQUAL(1, i.intRValueArg(0));
        ASSERT_EQUAL(2, i.compositeRValueArg(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(3, i.overloadedMethod(CompositeType{ 0, 1 }));
        ASSERT_EQUAL(std::make_pair(4, 5), i.overloadedMethod(0));
        ASSERT_EQUAL(&i, &i.operator=(i));
        ASSERT_EQUAL(&i, &i.operator=(std::move(i)));
    }

} __RValueTypesTests;
