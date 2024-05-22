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

struct MovingMocksAroundTests : tpunit::TestFixture
{

    MovingMocksAroundTests() :
        TestFixture(
            TEST(MovingMocksAroundTests::move_mock),
            TEST(MovingMocksAroundTests::move_mock_then_delete),
            TEST(MovingMocksAroundTests::create_mock_from_function),
            TEST(MovingMocksAroundTests::create_multiple_mocks_from_function)
        )
    {
    }

    class Interface
    {
    public:
        virtual std::string function(std::string) = 0;
        virtual ~Interface() = default;
    };

    Mock<Interface> createMock(const std::string& str)
    {
        Mock<Interface> mock;
        When(Method(mock, function)).AlwaysReturn(str);
        return mock;
    }

    void move_mock()
    {
        const std::string paramString = "long param string to not be in SSO ------------------------------------------";
        const std::string returnedString = "long returned string to not be in SSO ------------------------------------";

        Mock<Interface> mock;
        When(Method(mock, function)).AlwaysReturn(returnedString);

        Mock<Interface> mockMove = std::move(mock);

        EXPECT_EQUAL(mockMove.get().function(paramString), returnedString);

        Verify(Method(mockMove, function).Using(paramString)).Exactly(1);
    }

    void move_mock_then_delete()
    {
        const std::string paramString = "long param string to not be in SSO ------------------------------------------";
        const std::string returnedString = "long returned string to not be in SSO ------------------------------------";

        Mock<Interface> *mock = new Mock<Interface>;
        When(Method(*mock, function)).AlwaysReturn(returnedString);

        Mock<Interface> mockMove = std::move(*mock);
        delete mock;

        EXPECT_EQUAL(mockMove.get().function(paramString), returnedString);

        Verify(Method(mockMove, function).Using(paramString)).Exactly(1);
    }

    void create_mock_from_function()
    {
        const std::string paramString = "long param string to not be in SSO ------------------------------------------";
        const std::string returnedString = "long returned string to not be in SSO ------------------------------------";

        Mock<Interface> mock = createMock(returnedString);

        EXPECT_EQUAL(mock.get().function(paramString), returnedString);

        Verify(Method(mock, function).Using(paramString)).Exactly(1);
    }

    void create_multiple_mocks_from_function()
    {
        const std::string paramString1 = "long param 1 string to not be in SSO ---------------------------------------";
        const std::string returnedString1 = "long returned 1 string to not be in SSO ---------------------------------";
        const std::string paramString2 = "long param 2 string to not be in SSO ---------------------------------------";
        const std::string returnedString2 = "long returned 2 string to not be in SSO ---------------------------------";

        Mock<Interface> mock1 = createMock(returnedString1);
        Mock<Interface> mock2 = createMock(returnedString2);

        EXPECT_EQUAL(mock1.get().function(paramString1), returnedString1);
        EXPECT_EQUAL(mock2.get().function(paramString2), returnedString2);

        Verify(Method(mock1, function).Using(paramString1)).Exactly(1);
        Verify(Method(mock2, function).Using(paramString2)).Exactly(1);
    }

} __MovingMocksAroundTests;
