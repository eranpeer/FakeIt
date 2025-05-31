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
            TEST(ReturnTemplateSpecializationTests::return_ref_specialization_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_ref_capture_from_same_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_ref_specialization_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_ref_capture_from_other_type_temp),
            TEST(ReturnTemplateSpecializationTests::return_ref_default_from_variable),
            TEST(ReturnTemplateSpecializationTests::return_ref_specialization_from_variable),
            TEST(ReturnTemplateSpecializationTests::return_ref_capture_from_variable)
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
    };

    void return_ref_specialization_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).Return<std::string>(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).Return<MoveOnly>(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();
    }

    void return_ref_capture_from_same_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnCapture(std::string("something"));
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnCapture(MoveOnly{5});
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();
    }

    void return_ref_specialization_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).Return<std::string>("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).Return<MoveOnly>(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();
    }

    void return_ref_capture_from_other_type_temp() {
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnCapture("something");
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnCapture(5);
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();
    }

    void return_ref_default_from_variable() {
        std::string something = "something";
        MoveOnly mo = 5;
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).Return(something);
        something = "a different thing";
        ASSERT_EQUAL(mock.get().returnRef(), "a different thing");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).Return(mo);
        mo.i = 10;
        ASSERT_EQUAL(mock.get().returnRefMo().i, 10);
        Verify(Method(mock, returnRefMo)).Once();
    }

    void return_ref_specialization_from_variable() {
        std::string something = "something";
        MoveOnly mo = 5;
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).Return<std::string>(something);
        something = "a different thing";
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).Return<MoveOnly>(std::move(mo));
        mo.i = 10;
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();
    }

    void return_ref_capture_from_variable() {
        std::string something = "something";
        MoveOnly mo = 5;
        Mock<SomeStruct> mock;

        When(Method(mock, returnRef)).ReturnCapture(something);
        something = "a different thing";
        ASSERT_EQUAL(mock.get().returnRef(), "something");
        Verify(Method(mock, returnRef)).Once();

        When(Method(mock, returnRefMo)).ReturnCapture(std::move(mo));
        mo.i = 10;
        ASSERT_EQUAL(mock.get().returnRefMo().i, 5);
        Verify(Method(mock, returnRefMo)).Once();
    }

} __ReturnTemplateSpecializationTests;
