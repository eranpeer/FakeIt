#pragma once

namespace fakeit {

    template<typename R, typename... Args>
    struct Prototype;

    template<typename R, typename... Args>
    struct Prototype<R(Args...)> {

        template<class C>
        struct MemberType {

            using Type = R (C::*)(Args...);
            using ConstType = R (C::*)(Args...) const;
            using RefType = R (C::*)(Args...) &;
            using ConstRefType = R (C::*)(Args...) const&;
            using RValRefType = R (C::*)(Args...) &&;
            using ConstRValRefType = R (C::*)(Args...) const&&;

            static Type get(Type t) {
                return t;
            }

            static ConstType getConst(ConstType t) {
                return t;
            }

            static RefType getRef(RefType t) {
                return t;
            }

            static ConstRefType getConstRef(ConstRefType t) {
                return t;
            }

            static RValRefType getRValRef(RValRefType t) {
                return t;
            }

            static ConstRValRefType getConstRValRef(ConstRValRefType t) {
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
