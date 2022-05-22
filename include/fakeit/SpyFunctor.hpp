/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <type_traits>

#include "fakeit/StubbingProgress.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/SpyingContext.hpp"
#include "mockutils/type_utils.hpp"

namespace fakeit {

    class SpyFunctor {
    private:

        template<typename R, typename ... arglist, typename std::enable_if<all_true<std::is_copy_constructible<arglist>::value...>::value, int>::type = 0>
        void spy(const SpyingContext<R, arglist...> &root, int) {
            SpyingContext<R, arglist...> &rootWithoutConst = const_cast<SpyingContext<R, arglist...> &>(root);
            auto methodFromOriginalVT = rootWithoutConst.getOriginalMethodCopyArgs();
            rootWithoutConst.appendAction(new ReturnDelegateValue<R, arglist...>(methodFromOriginalVT));
            rootWithoutConst.commit();
        }

        template<typename R, typename ... arglist>
        void spy(const SpyingContext<R, arglist...> &, long) {
            static_assert(!std::is_same<R, R>::value, "Spy() cannot accept move-only args, use SpyWithoutVerify() instead which is able to forward these args but then they won't be available for Verify().");
        }

        void operator()() {
        }

    public:

        template<typename H, typename ... M>
        void operator()(const H &head, const M &... tail) {
            spy(head, 0);
            this->operator()(tail...);
        }

    };

}
