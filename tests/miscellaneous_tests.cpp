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

struct Miscellaneous : tpunit::TestFixture
{
    Miscellaneous() :
        TestFixture(
        TEST(Miscellaneous::pass_mock_by_ref), //
        TEST(Miscellaneous::can_mock_class_without_default_constructor), //
        TEST(Miscellaneous::can_mock_class_with_protected_constructor), //
        TEST(Miscellaneous::mock_virtual_methods_of_base_class), //
        TEST(Miscellaneous::create_and_delete_fakit_instatnce), //
        TEST(Miscellaneous::testStubFuncWithRightValueParameter), //
		TEST(Miscellaneous::testStubProcWithRightValueParameter),//
		TEST(Miscellaneous::aaa),//
		TEST(Miscellaneous::can_stub_method_after_reset),//
		TEST(Miscellaneous::method_is_still_stubbed_after_clear),//
		TEST(Miscellaneous::method_is_not_verified_after_clear)//
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

        struct Foo {
            virtual int bar(int &&) = 0;
        };

        Mock<Foo> foo_mock;
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

        struct Foo {
            virtual void bar(int &&) = 0;
        };

        int rv3 = 0;
        int rv4 = 0;
        int rv5 = 0;

        Mock<Foo> foo_mock;
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

    struct foo_bar {
      virtual ~foo_bar() {}
      virtual int foo() = 0;
      virtual int bar() = 0;
    };

    void can_stub_method_after_reset() {
      Mock<foo_bar> mock;
      mock.Reset();
      When(Method(mock, bar)).Return(42);
      ASSERT_EQUAL(mock.get().bar(), 42);
      Verify(Method(mock, bar)).Once();
    }

	void method_is_still_stubbed_after_clear() {
		Mock<foo_bar> mock;
		When(Method(mock, bar)).Return(42);
		mock.ClearInvocationHistory();
		ASSERT_EQUAL(mock.get().bar(), 42);
	}

	void method_is_not_verified_after_clear() {
		Mock<foo_bar> mock;
		When(Method(mock, bar)).Return(42);
		ASSERT_EQUAL(mock.get().bar(), 42);
		mock.ClearInvocationHistory();
		Verify(Method(mock, bar)).Never();
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


    class IBase
    {
    public:
        virtual void methodA() = 0;
    };

    class Base : public IBase
    {
    public:
        void methodA() override
        {
            std::cout << "Base::methodA";
        }
    };

    class Derived : public Base
    {
    public:
        void methodA() override
        {
            std::cout << "Derived::methodA";
        }
    };

    void foo()
    {
        Mock<Derived> mock;

        When(Method(mock, methodA)).AlwaysReturn();

        Derived& derived = mock.get();

        struct DoNotDelete { void operator()(Derived*) const { } };
        std::shared_ptr<Derived> derivedSPtr = std::shared_ptr<Derived>(&derived, DoNotDelete());

        //This version works #1
        //shared_ptr<Derived> derivedSPtr(new Derived());

        std::set<std::shared_ptr<Base>> set;

        set.insert(derivedSPtr);

        for (auto item : set)
        {
            std::shared_ptr<Base> b1 = item;
            b1->methodA();

            std::shared_ptr<Derived> d1 = std::dynamic_pointer_cast<Derived>(b1);
            //Next line fails.
            //d1 is always null for the mocked version. If a real Derived object is used like the line commented out above #1 it works fine.

            d1->methodA();
        }

    }


    class HandlerBase
    {};

    class TMessage {};

    class Handler : public HandlerBase
    {
    public:
        void Handle(TMessage message)
        {
            HandleMessage(message);
        }

        virtual void HandleMessage(TMessage) {};
    };

    void bar()
    {
        static_assert(std::is_polymorphic<Handler>::value, "Can only mock a polymorphic type");

    }

	class Product {};
	class IFactory {
	public:
		virtual std::unique_ptr<Product>Create() = 0;
	};
		
	void aaa()
	{
		Mock<IFactory> factory;
//		When(Method(factory, Create)).Return(std::make_unique<Product>());
//		auto a = factory.get().Create();
//		Verify(Method(factory, Create)).Once();
	}

} __Miscellaneous;
