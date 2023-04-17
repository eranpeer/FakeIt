/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct OverloadedMethods : tpunit::TestFixture {

    struct SomeInterface {
        virtual int func() = 0;

        virtual int func() const = 0;

        virtual int func(int) = 0;

        virtual int func(int, std::string) = 0;

        virtual const int func(double) = 0;

        virtual int& func(int, int) = 0;

        virtual int* func(int, int, int) = 0;

        virtual void proc() = 0;

        virtual void proc(const int&, std::string*) = 0;

    };

    OverloadedMethods() :
            tpunit::TestFixture(
                    TEST(OverloadedMethods::stub_overloaded_methods),
                    TEST(OverloadedMethods::stub_const_overloaded_methods),
                    TEST(OverloadedMethods::stub_overloaded_methods_with_templates<SomeInterface>),
                    TEST(OverloadedMethods::stub_modern_overloaded_methods),
                    TEST(OverloadedMethods::stub_modern_rvalref_overloaded_method),
                    TEST(OverloadedMethods::stub_modern_constrvalref_overloaded_method),
                    TEST(OverloadedMethods::stub_modern_overloaded_proc)
            ) {
    }

    void stub_overloaded_methods() {
        int x = 5;
        Mock<SomeInterface> mock;

        When(OverloadedMethod(mock, func, int())).Return(1);
        When(OverloadedMethod(mock, func, int(int))).Return(2);
        When(OverloadedMethod(mock, func, int(int, std::string))).Return(3);
        When(OverloadedMethod(mock, func, const int(double))).Return(4);
        When(OverloadedMethod(mock, func, int&(int,int) )).Return(x);
        When(OverloadedMethod(mock, func, int*(int, int, int))).Return(&x);

        When(OverloadedMethod(mock, proc, void())).Return();
        When(OverloadedMethod(mock, proc, void(const int&, std::string*))).Return();

        SomeInterface &i = mock.get();
        ASSERT_EQUAL(1, i.func());
        ASSERT_EQUAL(2, i.func(1));
        ASSERT_EQUAL(3, i.func(1, ""));
        ASSERT_EQUAL(4, i.func(1.0));
        ASSERT_EQUAL(5, i.func(1,1));
        ASSERT_EQUAL(5, *i.func(1, 1, 1));

        i.proc();
        i.proc(1,nullptr);

        When(OverloadedMethod(mock, func, int(int)).Using(1)).Return(10);
        ASSERT_EQUAL(10, i.func(1));
    }

    void stub_const_overloaded_methods() {
        Mock<SomeInterface> mock;

        When(OverloadedMethod(mock, func, int())).Return(1);
        When(ConstOverloadedMethod(mock, func, int())).Return(2);

        SomeInterface &obj = mock.get();
        const SomeInterface& constObj = mock.get();
        ASSERT_EQUAL(1, obj.func());
        ASSERT_EQUAL(2, constObj.func());
    }

    template <typename T>
    void stub_overloaded_methods_with_templates() {
        Mock<T> mock;
        T &i = mock.get();

        When(OverloadedMethod(mock, func, int(int))).AlwaysReturn(45);

        ASSERT_EQUAL(45, i.func(1));
    }

    struct SomeModernCppInterface {
        virtual int func() & = 0;

        virtual int func(int) & = 0;
        virtual int func(int) const& = 0;
        virtual int func(int) && = 0;
        virtual int func(int) const&& = 0;

        virtual void proc(int) & = 0;
        virtual void proc(int) const& = 0;
        virtual void proc(int) && = 0;
        virtual void proc(int) const&& = 0;
    };

    void stub_modern_overloaded_methods() {
        Mock<SomeModernCppInterface> mock;
        When(RefOverloadedMethod(mock, func, int(int))).Return(1);
        When(ConstRefOverloadedMethod(mock, func, int(int))).Return(2);

        SomeModernCppInterface& refObj = mock.get();
        const SomeModernCppInterface& refConstObj = mock.get();

        ASSERT_EQUAL(1, refObj.func(10));
        ASSERT_EQUAL(2, refConstObj.func(20));

        Verify(RefOverloadedMethod(mock, func, int(int)).Using(10)).Exactly(1);
        Verify(ConstRefOverloadedMethod(mock, func, int(int)).Using(20)).Exactly(1);

        VerifyNoOtherInvocations(mock);
    }

    void stub_modern_rvalref_overloaded_method() {
        Mock<SomeModernCppInterface> mock;
        When(RefOverloadedMethod(mock, func, int(int))).Return(1);
        When(RValRefOverloadedMethod(mock, func, int(int))).Return(3);

        SomeModernCppInterface& refObj = mock.get();

        ASSERT_EQUAL(1, refObj.func(1));
        ASSERT_EQUAL(3, std::move(refObj).func(1));

        Verify(RefOverloadedMethod(mock, func, int(int))).Exactly(1);
        Verify(RValRefOverloadedMethod(mock, func, int(int))).Exactly(1);

        VerifyNoOtherInvocations(mock);
    }

    void stub_modern_constrvalref_overloaded_method() {
        Mock<SomeModernCppInterface> mock;
        When(ConstRValRefOverloadedMethod(mock, func, int(int))).Return(4);

        const SomeModernCppInterface& refConstObj = mock.get();
        ASSERT_EQUAL(4, std::move(refConstObj).func(1));

        Verify(ConstRValRefOverloadedMethod(mock, func, int(int)).Using(1)).Exactly(1);

        VerifyNoOtherInvocations(mock);
    }

    void stub_modern_overloaded_proc() {
        Mock<SomeModernCppInterface> mock;
        int ret = 0;

        When(ConstRValRefOverloadedMethod(mock, proc, void(int))).Do([&](int){ ret = 4;});
        When(ConstRefOverloadedMethod(mock, proc, void(int))).Do([&](int){ret = 3;});
        When(RValRefOverloadedMethod(mock, proc, void(int))).Do([&](int){ ret = 2;});
        When(RefOverloadedMethod(mock, proc, void(int))).Do([&](int){ret = 1;});

        SomeModernCppInterface& refObj = mock.get();
        const SomeModernCppInterface& refConstObj = mock.get();

        refObj.proc(0);
        ASSERT_EQUAL(1, ret);
        std::move(refObj).proc(0);
        ASSERT_EQUAL(2, ret);
        refConstObj.proc(0);
        ASSERT_EQUAL(3, ret);
        std::move(refConstObj).proc(0);
        ASSERT_EQUAL(4, ret);

        Verify(RefOverloadedMethod(mock, proc, void(int))).Exactly(1);
        Verify(RValRefOverloadedMethod(mock, proc, void(int))).Exactly(1);
        Verify(ConstRefOverloadedMethod(mock, proc, void(int))).Exactly(1);
        Verify(ConstRValRefOverloadedMethod(mock, proc, void(int))).Exactly(1);

        VerifyNoOtherInvocations(mock);
    }

} __OverloadedMethods;
