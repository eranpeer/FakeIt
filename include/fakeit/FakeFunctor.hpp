/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "fakeit/StubbingProgress.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/StubbingContext.hpp"
#include "fakeit/FakeitContext.hpp"

namespace fakeit {

    class FakeFunctor {
    private:
        template<typename R, typename ... arglist>
        void fake(const StubbingContext<R, arglist...> &root) {
            StubbingContext<R, arglist...> &rootWithoutConst = const_cast<StubbingContext<R, arglist...> &>(root);
            rootWithoutConst.appendAction(new ReturnDefaultValue<R, arglist...>());
            rootWithoutConst.commit();
        }

        void operator()() {
        }

    public:

        template<typename H, typename ... M>
        void operator()(const H &head, const M &... tail) {
            fake(head);
            this->operator()(tail...);
        }

    };

}
