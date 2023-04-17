/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "fakeit/DomainObjects.hpp"
#include "fakeit/MockImpl.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Prototype.hpp"

namespace fakeit {
    namespace internal {
        template<typename T, typename = void>
        struct WithCommonVoid {
            using type = T;
        };

        // The goal of this specialization is to replace all kinds of void (e.g. "const void") by a common type, "void".
        // This is because some other parts of the code have specialization for "void", but these specializations only
        // handle "void", and not "const void", so instead of modifying all these specialization to handle both types,
        // we replace "const void" by "void" here.
        template<typename T>
        struct WithCommonVoid<T, typename std::enable_if<std::is_void<T>::value, void>::type> {
            using type = void;
        };

        template<typename T>
        using WithCommonVoid_t = typename WithCommonVoid<T>::type;
    }

    template<typename C, typename ... baseclasses>
    class Mock : public ActualInvocationsSource {
        MockImpl<C, baseclasses...> impl;
    public:
        virtual ~Mock() = default;

        static_assert(std::is_polymorphic<C>::value, "Can only mock a polymorphic type");

        Mock() : impl(Fakeit) {
        }

        explicit Mock(C &obj) : impl(Fakeit, obj) {
        }

        virtual C &get() {
            return impl.get();
        }

//		std::shared_ptr<C> getShared() {
//			return impl.getShared();
//		}

		C &operator()() {
            return get();
        }

        void Reset() {
            impl.reset();
        }

		void ClearInvocationHistory() {
			impl.clear();
		}

        template<class DataType, typename ... arglist,
                class = typename std::enable_if<std::is_member_object_pointer<DataType C::*>::value>::type>
        DataMemberStubbingRoot<C, DataType> Stub(DataType C::* member, const arglist &... ctorargs) {
            return impl.stubDataMember(member, ctorargs...);
        }

        // const
        template<int id, typename R, typename T, typename ... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R (T::*vMethod)(arglist...) const) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R> (T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // volatile
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...) volatile) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // const volatile
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...) const volatile) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // no qualifier
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...)) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // ref
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...) &) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // const ref
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...) const&) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // rval ref
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...) &&) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        // const rval ref
        template<int id, typename R, typename T, typename... arglist, class = typename std::enable_if<
                std::is_base_of<T, C>::value>::type>
        MockingContext<internal::WithCommonVoid_t<R>, arglist...> stub(R(T::*vMethod)(arglist...) const&&) {
            auto methodWithoutConstVolatile = reinterpret_cast<internal::WithCommonVoid_t<R>(T::*)(arglist...)>(vMethod);
            return impl.template stubMethod<id>(methodWithoutConstVolatile);
        }

        DtorMockingContext dtor() {
            return impl.stubDtor();
        }

        void getActualInvocations(std::unordered_set<Invocation *> &into) const override {
            impl.getActualInvocations(into);
        }

    };

}
