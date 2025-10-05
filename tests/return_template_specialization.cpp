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
            TEST(ReturnTemplateSpecializationTests::return_valcapt_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_default_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_valspecialization_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_valcapt_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_default_from_same_type_variable),
            TEST(ReturnTemplateSpecializationTests::return_valspecialization_from_same_type_variable),
            TEST(ReturnTemplateSpecializationTests::return_valcapt_from_same_type_variable),
            TEST(ReturnTemplateSpecializationTests::return_default_from_other_type_variable),
            TEST(ReturnTemplateSpecializationTests::return_valspecialization_from_other_type_variable),
            TEST(ReturnTemplateSpecializationTests::return_valcapt_from_other_type_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_valcapt_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_valcapt_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_same_type_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_same_type_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_valcapt_from_same_type_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_default_from_other_type_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_valspecialization_from_other_type_variable),
            TEST(ReturnTemplateSpecializationTests::always_return_valcapt_from_other_type_variable)
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

    void return_valcapt_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnValCapt(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnValCapt(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();

        When(Method(mock, returnVal)).ReturnValCapt(std::string("something"));
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).ReturnValCapt(MoveOnly{5});
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

    void return_valcapt_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnValCapt("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnValCapt(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();

        When(Method(mock, returnVal)).ReturnValCapt("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Once();

        When(Method(mock, returnValMo)).ReturnValCapt(5);
        ASSERT_EQUAL(mock.get().returnValMo().i, 5);
        Verify(Method(mock, returnValMo)).Once();
    }

    void return_default_from_same_type_variable() {
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

    void return_valspecialization_from_same_type_variable() {
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

    void return_valcapt_from_same_type_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).ReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Once();

            When(Method(mock, returnRefMo)).ReturnValCapt(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Once();
        }

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnVal)).ReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).ReturnValCapt(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnValMo().i, 5);
            Verify(Method(mock, returnValMo)).Once();
        }
    }

    void return_default_from_other_type_variable() {
        Mock<SomeStruct> mock;

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnVal)).Return(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).Return(num);
            num = 10;
            ASSERT_EQUAL(mock.get().returnValMo().i, 5);
            Verify(Method(mock, returnValMo)).Once();
        }
    }

    void return_valspecialization_from_other_type_variable() {
        Mock<SomeStruct> mock;

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnRef)).Return<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Once();

            When(Method(mock, returnRefMo)).Return<MoveOnly>(num);
            num = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Once();
        }

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnVal)).Return<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).Return<MoveOnly>(num);
            num = 10;
            ASSERT_EQUAL(mock.get().returnValMo().i, 5);
            Verify(Method(mock, returnValMo)).Once();
        }
    }

    void return_valcapt_from_other_type_variable() {
        Mock<SomeStruct> mock;

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnRef)).ReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Once();

            When(Method(mock, returnRefMo)).ReturnValCapt(num);
            num = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Once();
        }

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnVal)).ReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Once();

            When(Method(mock, returnValMo)).ReturnValCapt(num);
            num = 10;
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

    void always_return_valcapt_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).AlwaysReturnValCapt(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Exactly(2);

        When(Method(mock, returnRefMo)).AlwaysReturnValCapt(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Exactly(2);

        When(Method(mock, returnVal)).AlwaysReturnValCapt(std::string("something"));
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

    void always_return_valcapt_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).AlwaysReturnValCapt("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Exactly(2);

        When(Method(mock, returnRefMo)).AlwaysReturnValCapt(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Exactly(2);

        When(Method(mock, returnVal)).AlwaysReturnValCapt("something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        ASSERT_EQUAL(mock.get().returnVal(), "something");
        Verify(Method(mock, returnVal)).Exactly(2);
    }

    void always_return_default_from_same_type_variable() {
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

    void always_return_valspecialization_from_same_type_variable() {
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

    void always_return_valcapt_from_same_type_variable() {
        Mock<SomeStruct> mock;

        {
            std::string something = "something";
            MoveOnly mo = 5;

            When(Method(mock, returnRef)).AlwaysReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Exactly(2);

            When(Method(mock, returnRefMo)).AlwaysReturnValCapt(std::move(mo));
            mo.i = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            mo.i = 50;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Exactly(2);
        }

        {
            std::string something = "something";

            When(Method(mock, returnVal)).AlwaysReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

    void always_return_default_from_other_type_variable() {
        Mock<SomeStruct> mock;

        {
            const char* something = "something";

            When(Method(mock, returnVal)).AlwaysReturn(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

    void always_return_valspecialization_from_other_type_variable() {
        Mock<SomeStruct> mock;

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnRef)).AlwaysReturn<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Exactly(2);

            When(Method(mock, returnRefMo)).AlwaysReturn<MoveOnly>(num);
            num = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            num = 50;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Exactly(2);
        }

        {
            const char* something = "something";

            When(Method(mock, returnVal)).AlwaysReturn<std::string>(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

    void always_return_valcapt_from_other_type_variable() {
        Mock<SomeStruct> mock;

        {
            const char* something = "something";
            int num = 5;

            When(Method(mock, returnRef)).AlwaysReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnRef(), "something");
            Verify(Method(mock, returnRef)).Exactly(2);

            When(Method(mock, returnRefMo)).AlwaysReturnValCapt(num);
            num = 10;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            num = 50;
            ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
            Verify(Method(mock, returnRefMo)).Exactly(2);
        }

        {
            const char* something = "something";

            When(Method(mock, returnVal)).AlwaysReturnValCapt(something);
            something = "a different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            something = "another different thing";
            ASSERT_EQUAL(mock.get().returnVal(), "something");
            Verify(Method(mock, returnVal)).Exactly(2);
        }
    }

} __ReturnTemplateSpecializationTests;
