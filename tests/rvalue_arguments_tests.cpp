#include <string>
#include <queue>
#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

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
        TestFixture(
        //
        TEST(RValueTypesTests::explicitStubbingDefaultReturnValues),
        TEST(RValueTypesTests::explicitStubbingReturnValues_with_AlwaysReturn),
        TEST(RValueTypesTests::explicitStubbingReturnValues_by_assignment),
        TEST(RValueTypesTests::explicitStubbingReturnValues)
        //
        ) {
    }

    void explicitStubbingDefaultReturnValues() {
        fakeit::Mock<RValueInterface> mock;     //
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
        fakeit::Mock<RValueInterface> mock;     //
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