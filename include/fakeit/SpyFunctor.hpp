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
#include "fakeit/SpyingContext.hpp"

namespace fakeit {

    class SpyFunctor {
    private:

        template<typename R, typename ... arglist>
        void spy(const SpyingContext<R, arglist...> &root) {
            SpyingContext<R, arglist...> &rootWithoutConst = const_cast<SpyingContext<R, arglist...> &>(root);
            auto methodFromOriginalVT = rootWithoutConst.getOriginalMethod();
            rootWithoutConst.appendAction(new ReturnDelegateValue<R, arglist...>(methodFromOriginalVT));
            rootWithoutConst.commit();
        }

        void operator()() {
        }

    public:

        template<typename H, typename ... M>
        void operator()(const H &head, const M &... tail) {
            spy(head);
            this->operator()(tail...);
        }

    };

}
