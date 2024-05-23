#pragma once

namespace fakeit {

    template<typename R, typename... Args>
    struct Prototype;

    template<typename R, typename... Args>
    struct Prototype<R(Args...)> {

        template<class C>
        using Type = R (C::*)(Args...);
        template<class C>
        using ConstType = R (C::*)(Args...) const;
        template<class C>
        using RefType = R (C::*)(Args...) &;
        template<class C>
        using ConstRefType = R (C::*)(Args...) const&;
        template<class C>
        using RValRefType = R (C::*)(Args...) &&;
        template<class C>
        using ConstRValRefType = R (C::*)(Args...) const&&;

        template <typename DefaultC>
        struct MemberType {

            template<class C>
            static constexpr Type<C> get(Type<C> t) {
                return t;
            }

            static constexpr Type<DefaultC> get(Type<DefaultC> t) {
                return t;
            }

            template<class C>
            static constexpr ConstType<C> getConst(ConstType<C> t) {
                return t;
            }

            static constexpr ConstType<DefaultC> getConst(ConstType<DefaultC> t) {
                return t;
            }

            template<class C>
            static constexpr RefType<C> getRef(RefType<C> t) {
                return t;
            }

            static constexpr RefType<DefaultC> getRef(RefType<DefaultC> t) {
                return t;
            }

            template<class C>
            static constexpr ConstRefType<C> getConstRef(ConstRefType<C> t) {
                return t;
            }

            static constexpr ConstRefType<DefaultC> getConstRef(ConstRefType<DefaultC> t) {
                return t;
            }

            template<class C>
            static constexpr RValRefType<C> getRValRef(RValRefType<C> t) {
                return t;
            }

            static constexpr RValRefType<DefaultC> getRValRef(RValRefType<DefaultC> t) {
                return t;
            }

            template<class C>
            static constexpr ConstRValRefType<C> getConstRValRef(ConstRValRefType<C> t) {
                return t;
            }

            static constexpr ConstRValRefType<DefaultC> getConstRValRef(ConstRValRefType<DefaultC> t) {
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
