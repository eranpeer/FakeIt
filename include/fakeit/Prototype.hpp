#pragma once

namespace fakeit {

    template<typename R, typename... Args>
    struct Prototype;

    template<typename R, typename... Args>
    struct Prototype<R(Args...)> {

        typedef R Type(Args...);

        typedef R ConstType(Args...) const;

        template<class C>
        struct MemberType {

            typedef Type(C::*type);
            typedef ConstType(C::*cosntType);

            static type get(type t) {
                return t;
            }

            static cosntType getconst(cosntType t) {
                return t;
            }

        };

    };

    template<int X, typename R, typename C, typename... arglist>
    struct UniqueMethod {
        R (C::*method)(arglist...);

        UniqueMethod(R (C::*vMethod)(arglist...)) : method(vMethod) { }

        int uniqueId() {
            return X;
        }
//        typedef typename std::remove_cv<R>::type naked_return_type;
//        typedef naked_return_type method_type(Args...);
//        //typedef naked_return_type(C::*member_type)(Args...);
//        typedef method_type(C::*member_type);
    };

}

