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

	class MoveOnlyType {
	public:
		int state;
		MoveOnlyType(int value) :
				state(value) {
		}
		MoveOnlyType(const MoveOnlyType&) = delete;
		MoveOnlyType(MoveOnlyType&&) = default;

		bool operator==(const MoveOnlyType& other) const {
			return (other.state == this->state);
		}

	};

	struct MoveOnlyInterface {
		virtual std::string returnCopyable() = 0;
		virtual std::unique_ptr<std::string> returnMoveOnlyUniqueString() = 0;
		virtual MoveOnlyType returnMoveOnlyType() = 0;
		virtual std::vector<MoveOnlyType> returnVectorOfMoveOnly() = 0;
	};

	static std::vector<MoveOnlyType> constructVectorOfMoveOnly(int i) {
		std::vector<MoveOnlyType> vectorOfMoveOnly;
		vectorOfMoveOnly.emplace_back(i);
		return vectorOfMoveOnly;
	}

	MoveOnlyReturnTests() :
			tpunit::TestFixture(
					//
					TEST(MoveOnlyReturnTests::explicitStubbingReturnValuesFromTemporary),
					TEST(MoveOnlyReturnTests::explicitStubbingReturnValuesFromMove),
					TEST(MoveOnlyReturnTests::explicitStubbingReturnMultipleValuesMoveAndCopy)
					//
							) {
	}

	void explicitStubbingReturnValuesFromTemporary() {
		Mock<MoveOnlyInterface> mock;

		When(Method(mock, returnMoveOnlyUniqueString)).Return(std::unique_ptr<std::string>(new std::string("value")));
		When(Method(mock, returnMoveOnlyType)).Return(MoveOnlyType(10));
		When(Method(mock, returnVectorOfMoveOnly)).Return(constructVectorOfMoveOnly(5));

		MoveOnlyInterface & i = mock.get();

		ASSERT_EQUAL("value", *i.returnMoveOnlyUniqueString());
		ASSERT_EQUAL(MoveOnlyType(10), i.returnMoveOnlyType());
		ASSERT_EQUAL(constructVectorOfMoveOnly(5), i.returnVectorOfMoveOnly());
	}

	void explicitStubbingReturnValuesFromMove() {
		Mock<MoveOnlyInterface> mock;

		std::string str{"copyable"};
		std::unique_ptr<std::string> strPtr(new std::string("value"));
		MoveOnlyType moveOnly(10);
		std::vector<MoveOnlyType> vectorOfMoveOnly = constructVectorOfMoveOnly(5);

		When(Method(mock, returnCopyable)).Return(std::move(str));
		When(Method(mock, returnMoveOnlyUniqueString)).Return(std::move(strPtr));
		When(Method(mock, returnMoveOnlyType)).Return(std::move(moveOnly));
		When(Method(mock, returnVectorOfMoveOnly)).Return(std::move(vectorOfMoveOnly));

		// check move did happen
		ASSERT_TRUE(str.empty());
		ASSERT_EQUAL(strPtr, nullptr);
		ASSERT_TRUE(vectorOfMoveOnly.empty());

		MoveOnlyInterface& i = mock.get();

		ASSERT_EQUAL(std::string("copyable"), i.returnCopyable());
		ASSERT_EQUAL(std::string("value"), *i.returnMoveOnlyUniqueString());
		ASSERT_EQUAL(MoveOnlyType(10), i.returnMoveOnlyType());
		ASSERT_EQUAL(constructVectorOfMoveOnly(5), i.returnVectorOfMoveOnly());
	}

	void explicitStubbingReturnMultipleValuesMoveAndCopy() {
		Mock<MoveOnlyInterface> mock;

		std::string copiedStr{"copied"};
		std::string movedStr{"moved"};
		std::unique_ptr<std::string> strPtr(new std::string("strPtrMove"));
		MoveOnlyType moveOnly(100);
		std::vector<MoveOnlyType> vectorOfMoveOnly = constructVectorOfMoveOnly(50);

		When(Method(mock, returnCopyable)).Return(copiedStr, std::move(movedStr));
		When(Method(mock, returnMoveOnlyUniqueString)).Return(std::unique_ptr<std::string>(new std::string("strPtrRval")), std::move(strPtr));
		When(Method(mock, returnMoveOnlyType)).Return(MoveOnlyType(10), std::move(moveOnly));
		When(Method(mock, returnVectorOfMoveOnly)).Return(constructVectorOfMoveOnly(5), std::move(vectorOfMoveOnly));

		ASSERT_EQUAL(copiedStr, "copied"); // check move did NOT happen
		// check move did happen
		ASSERT_TRUE(movedStr.empty());
		ASSERT_EQUAL(strPtr, nullptr);
		ASSERT_TRUE(vectorOfMoveOnly.empty());

		MoveOnlyInterface& i = mock.get();

		ASSERT_EQUAL(std::string("copied"), i.returnCopyable());
		ASSERT_EQUAL(std::string("moved"), i.returnCopyable());
		ASSERT_EQUAL(std::string("strPtrRval"), *i.returnMoveOnlyUniqueString());
		ASSERT_EQUAL(std::string("strPtrMove"), *i.returnMoveOnlyUniqueString());
		ASSERT_EQUAL(MoveOnlyType(10), i.returnMoveOnlyType());
		ASSERT_EQUAL(MoveOnlyType(100), i.returnMoveOnlyType());
		ASSERT_EQUAL(constructVectorOfMoveOnly(5), i.returnVectorOfMoveOnly());
		ASSERT_EQUAL(constructVectorOfMoveOnly(50), i.returnVectorOfMoveOnly());
	}
} __MoveOnlyReturnTests;
