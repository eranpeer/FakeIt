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

    OverloadedMethods() :
            tpunit::TestFixture(
                    TEST(OverloadedMethods::stub_overloaded_methods),
                    TEST(OverloadedMethods::stub_const_overloaded_methods)) {
    }

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

} __OverloadedMethods;
