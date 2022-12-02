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

struct MoveOnlyReturnTests: tpunit::TestFixture {

	class AbstractType {
	public:
		virtual void foo() = 0;
	};

	class ConcreteType: public AbstractType {
	public:
		int state;
		ConcreteType(int value) :
				state(value) {
		}
		ConcreteType(const ConcreteType&) = delete;
		ConcreteType(ConcreteType&&) = default;

		virtual void foo() override {
		}

		bool operator==(const ConcreteType& other) {
			return (other.state == this->state);
		}

	};

	struct ReferenceInterface {
		virtual std::unique_ptr<std::string> returnMoveOnlyUniqueString() = 0;
		virtual ConcreteType returnMoveOnlyConcreteTypeByRef() = 0;
	};

	MoveOnlyReturnTests() :
			tpunit::TestFixture(
					//
					TEST(MoveOnlyReturnTests::explicitStubbingReturnValuesFromTemporary),
					TEST(MoveOnlyReturnTests::explicitStubbingReturnValuesFromMove)
					//
							) {
	}

	void explicitStubbingReturnValuesFromTemporary() {
		Mock<ReferenceInterface> mock;

		When(Method(mock, returnMoveOnlyUniqueString)).Return(std::unique_ptr<std::string>(new std::string("value")));
		When(Method(mock, returnMoveOnlyConcreteTypeByRef)).Return(ConcreteType(10));

		ReferenceInterface & i = mock.get();

		ASSERT_EQUAL("value", *i.returnMoveOnlyUniqueString());

		ASSERT_EQUAL(ConcreteType(10), i.returnMoveOnlyConcreteTypeByRef());
	}

	void explicitStubbingReturnValuesFromMove() {
		Mock<ReferenceInterface> mock;

		ConcreteType c(10);
		std::unique_ptr<std::string> string(new std::string("value"));

		When(Method(mock, returnMoveOnlyUniqueString)).Return(std::move(string));
		When(Method(mock, returnMoveOnlyConcreteTypeByRef)).Return(std::move(c));

		ASSERT_FALSE(string); // check move did happen

		ReferenceInterface& i = mock.get();

		ASSERT_EQUAL(std::string("value"), *i.returnMoveOnlyUniqueString());

		ASSERT_EQUAL(ConcreteType(10), i.returnMoveOnlyConcreteTypeByRef());
	}
} __MoveOnlyReturnTests;

