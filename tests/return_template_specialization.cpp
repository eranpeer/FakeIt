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

struct ReturnTemplateSpecializationTests : tpunit::TestFixture {

    ReturnTemplateSpecializationTests()
        : tpunit::TestFixture(
            TEST(ReturnTemplateSpecializationTests::return_default_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_valspecialization_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_capture_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_default_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_valspecialization_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_capture_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_default_from_variable),
            TEST(ReturnTemplateSpecializationTests::return_valspecialization_from_variable),
            TEST(ReturnTemplateSpecializationTests::return_capture_from_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_capture_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_capture_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_capture_from_variable)
        ) {
    }

    struct MoveOnly {
        int i = 0;
        MoveOnly(int pi) : i{pi} {}
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&&) = default;
    };

    struct SomeStruct {
        virtual ~SomeStruct() = default;

        virtual const std::string& returnRef() = 0;

        virtual const MoveOnly& returnRefMo() = 0;

        virtual std::string returnVal() = 0;

        virtual MoveOnly returnValMo() = 0;
    };

    void return_default_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnVal)).Return(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).Return(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_valspecialization_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).Return<std::string>(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).Return<MoveOnly>(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();

        When(Method(mock, returnVal)).Return<std::string>(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).Return<MoveOnly>(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_capture_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnCapture(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnCapture(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();

        When(Method(mock, returnVal)).ReturnCapture(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).ReturnCapture(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_default_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnVal)).Return("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).Return(5);
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_valspecialization_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).Return<std::string>("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).Return<MoveOnly>(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();

        When(Method(mock, returnVal)).Return<std::string>("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).Return<MoveOnly>(5);
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_capture_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnCapture("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnCapture(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();

        When(Method(mock, returnVal)).ReturnCapture("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).ReturnCapture(5);
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_default_from_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).Return(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "a different thing");
            Verify(Method(mock, returnRef)).Once();

            When(Method(mock, returnRefMo)).Return(mo);
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 10);
            Verify(Method(mock, returnRefMo)).Once();
        }

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnVal)).Return(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).Return(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnValMo().i, 5);
            Verify(Method(mock, returnValMo)).Once();
        }
    }

    void return_valspecialization_from_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).Return<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Once();

            When(Method(mock, returnRefMo)).Return<MoveOnly>(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Once();
        }

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnVal)).Return<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).Return<MoveOnly>(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnValMo().i, 5);
            Verify(Method(mock, returnValMo)).Once();
        }
    }

    void return_capture_from_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).ReturnCapture(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Once();

            When(Method(mock, returnRefMo)).ReturnCapture(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Once();
        }

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnVal)).ReturnCapture(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).ReturnCapture(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnValMo().i, 5);
            Verify(Method(mock, returnValMo)).Once();
        }
    }

    void always_return_default_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnVal)).AlwaysReturn(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_valspecialization_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).AlwaysReturn<std::string>(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Exactly(2);

        When(Method(mock, returnRefMo)).AlwaysReturn<MoveOnly>(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Exactly(2);

        When(Method(mock, returnVal)).AlwaysReturn<std::string>(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_capture_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).AlwaysReturnCapture(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Exactly(2);

        When(Method(mock, returnRefMo)).AlwaysReturnCapture(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Exactly(2);

        When(Method(mock, returnVal)).AlwaysReturnCapture(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_default_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnVal)).AlwaysReturn("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_valspecialization_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).AlwaysReturn<std::string>("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Exactly(2);

        When(Method(mock, returnRefMo)).AlwaysReturn<MoveOnly>(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Exactly(2);

        When(Method(mock, returnVal)).AlwaysReturn<std::string>("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_capture_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).AlwaysReturnCapture("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Exactly(2);

        When(Method(mock, returnRefMo)).AlwaysReturnCapture(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Exactly(2);

        When(Method(mock, returnVal)).AlwaysReturnCapture("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_default_from_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).AlwaysReturn(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "a different thing");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "another different thing");
            Verify(Method(mock, returnRef)).Exactly(2);

            When(Method(mock, returnRefMo)).AlwaysReturn(mo);
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 10);
            mo.i = 50;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 50);
            Verify(Method(mock, returnRefMo)).Exactly(2);
        }

        {
            std::string something = "something";

            When(Method(mock, returnVal)).AlwaysReturn(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

    void always_return_valspecialization_from_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).AlwaysReturn<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Exactly(2);

            When(Method(mock, returnRefMo)).AlwaysReturn<MoveOnly>(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            mo.i = 50;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Exactly(2);
        }

        {
            std::string something = "something";

            When(Method(mock, returnVal)).AlwaysReturn<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

    void always_return_capture_from_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).AlwaysReturnCapture(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Exactly(2);

            When(Method(mock, returnRefMo)).AlwaysReturnCapture(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            mo.i = 50;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Exactly(2);
        }

        {
            std::string something = "something";

            When(Method(mock, returnVal)).AlwaysReturnCapture(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

} __ReturnTemplateSpecializationTests;
