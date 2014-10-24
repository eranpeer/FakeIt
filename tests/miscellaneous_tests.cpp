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
#include "fakeit/FakeitExceptions.hpp"

using namespace fakeit;

struct Miscellaneous: tpunit::TestFixture
{
	Miscellaneous() :
		TestFixture(
			TEST(Miscellaneous::pass_mock_by_ref), //
			TEST(Miscellaneous::can_mock_class_without_default_constructor), //
			TEST(Miscellaneous::can_mock_class_with_protected_constructor), //
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
		{
			StandaloneFakeit df;
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
} __Miscellaneous;