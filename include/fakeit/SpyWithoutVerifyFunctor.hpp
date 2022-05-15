#pragma once

#include "fakeit/StubbingProgress.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/SpyingContext.hpp"

namespace fakeit {

    class SpyWithoutVerifyFunctor {
    private:

        template<typename R, typename ... arglist>
        void spy(const SpyingContext<R, arglist...> &root) {
            SpyingContext<R, arglist...> &rootWithoutConst = const_cast<SpyingContext<R, arglist...> &>(root);
            auto methodFromOriginalVT = rootWithoutConst.getOriginalMethodForwardArgs();
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
