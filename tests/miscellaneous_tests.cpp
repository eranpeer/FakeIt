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

struct Miscellaneous: tpunit::TestFixture
{
	Miscellaneous() :
		TestFixture(
			TEST(Miscellaneous::pass_mock_by_ref), //
			TEST(Miscellaneous::can_mock_class_without_default_constructor), //
			TEST(Miscellaneous::can_mock_class_with_protected_constructor), //
			TEST(Miscellaneous::mock_virtual_methods_of_base_class), //
			TEST(Miscellaneous::create_and_delete_fakit_instatnce) //
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
		Fake(Method(mock,foo));
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
		Fake(Method(mock,foo));
	}

	void create_and_delete_fakit_instatnce()
	{
        class MyFakeit:public DefaultFakeit {
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
		virtual void change(uint8_t r, uint8_t g, uint8_t b) =0;
	};

	void assertChanged(Mock<Change>& mock, uint8_t v1, uint8_t v2, uint8_t v3)
	{
		Verify(Method(mock, change).Using(v1,v2,v3));
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

    struct B: public A
    {
		virtual ~B() {}
        virtual int b1() = 0;
        virtual int b2() = 0;
    };

    void mock_virtual_methods_of_base_class() {
		Mock<B> mock;
        When(Method(mock,b1)).Return(1);
        When(Method(mock,b2)).Return(2);
        When(Method(mock,a1)).Return(3);
        When(Method(mock,a2)).Return(4);

        ASSERT_EQUAL(1, mock().b1());
        ASSERT_EQUAL(2, mock().b2());
        ASSERT_EQUAL(3, mock().a1());
        ASSERT_EQUAL(4, mock().a2());
	}

} __Miscellaneous;
