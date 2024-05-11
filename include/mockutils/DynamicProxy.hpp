/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#undef max
#include <functional>
#include <type_traits>
#include <vector>
#include <array>
#include <new>
#include <limits>

#include "mockutils/VirtualTable.hpp"
#include "mockutils/union_cast.hpp"
#include "mockutils/MethodInvocationHandler.hpp"
#include "mockutils/VTUtils.hpp"
#include "mockutils/FakeObject.hpp"
#include "mockutils/Finally.hpp"
#include "mockutils/MethodProxy.hpp"
#include "mockutils/MethodProxyCreator.hpp"

namespace fakeit {

    class InvocationHandlers : public InvocationHandlerCollection {
        std::vector<std::shared_ptr<Destructible>> &_methodMocks;
        std::vector<size_t> &_offsets;

        unsigned int getOffset(size_t id) const
        {
            unsigned int offset = 0;
            for (; offset < _offsets.size(); offset++) {
                if (_offsets[offset] == id) {
                    break;
                }
            }
            return offset;
        }

    public:
        InvocationHandlers(std::vector<std::shared_ptr<Destructible>> &methodMocks, std::vector<size_t> &offsets)
                : _methodMocks(methodMocks), _offsets(offsets) {
        }

        Destructible *getInvocatoinHandlerPtrById(size_t id) override {
            unsigned int offset = getOffset(id);
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return ptr.get();
        }

    };

    template<typename C, typename ... baseclasses>
    struct DynamicProxy {

        static_assert(std::is_polymorphic<C>::value, "DynamicProxy requires a polymorphic type");

        DynamicProxy(C &inst) :
                _instancePtr(&inst),
                _methodMocks(VTUtils::getVTSize<C>()),
                _offsets(VTUtils::getVTSize<C>(), std::numeric_limits<int>::max()),
                _invocationHandlers(_methodMocks, _offsets) {
            _originalVt.copyFrom(VirtualTable<C, baseclasses...>::getVTable(*_instancePtr));
            _originalVt.setCookie(InvocationHandlerCollection::VtCookieIndex, &_invocationHandlers);
            getFake().swapVirtualTable(_originalVt);
        }

        DynamicProxy(const DynamicProxy&) = delete;
        DynamicProxy(DynamicProxy&& other) FAKEIT_NO_THROWS
            : _originalVt(std::move(other._originalVt))
            , _methodMocks(std::move(other._methodMocks))
            , _members(std::move(other._members))
            , _offsets(std::move(other._offsets))
            , _invocationHandlers(_methodMocks, _offsets) {
            std::swap(_instancePtr, other._instancePtr);
            VirtualTable<C, baseclasses...>::getVTable(*_instancePtr).setCookie(InvocationHandlerCollection::VtCookieIndex, &_invocationHandlers);
        }

        DynamicProxy& operator=(const DynamicProxy&) = delete;
        DynamicProxy& operator=(DynamicProxy&&) = delete;

        ~DynamicProxy() = default;

        void detach() {
            if (_instancePtr != nullptr) {
                getFake().swapVirtualTable(_originalVt);
            }
        }

        C &get() {
            return *_instancePtr;
        }

        void Reset() {
			_methodMocks = {};
            _methodMocks.resize(VTUtils::getVTSize<C>());
            _members = {};
			_offsets = {};
            _offsets.resize(VTUtils::getVTSize<C>());
            VirtualTable<C, baseclasses...>::getVTable(*_instancePtr).copyFrom(_originalVt);
        }

		void Clear()
        {
        }

        template<size_t id, typename R, typename ... arglist>
        void stubMethod(R(C::*vMethod)(arglist...), MethodInvocationHandler<R, arglist...> *methodInvocationHandler) {
            auto offset = VTUtils::getOffset(vMethod);
            MethodProxyCreator<R, arglist...> creator;
            bind(creator.template createMethodProxy<id + 1>(offset), methodInvocationHandler);
        }

        void stubDtor(MethodInvocationHandler<void> *methodInvocationHandler) {
            auto offset = VTUtils::getDestructorOffset<C>();
            MethodProxyCreator<void> creator;
            // MSVC use an indirection for destructors, the "initial" destructor (VirtualTable::dtor, an helper) will be
            // called through a member-function call, but the "final" destructor (the method proxy) will be called through
            // a free-function call (inside the initial destructor). Therefor we use the free-function version
            // (static method, but it's the same) of MethodProxy.
            // For GCC / Clang, the destructor is directly called, like normal methods, so we use the member-function
            // version.
#ifdef _MSC_VER
            bindDtor(creator.createMethodProxyStatic<0>(offset), methodInvocationHandler);
#else
            bindDtor(creator.createMethodProxy<0>(offset), methodInvocationHandler);
#endif
        }

        template<typename R, typename ... arglist>
        bool isMethodStubbed(R(C::*vMethod)(arglist...)) {
            unsigned int offset = VTUtils::getOffset(vMethod);
            return isBinded(offset);
        }

        bool isDtorStubbed() {
            unsigned int offset = VTUtils::getDestructorOffset<C>();
            return isBinded(offset);
        }

        template<typename R, typename ... arglist>
        Destructible *getMethodMock(R(C::*vMethod)(arglist...)) {
            auto offset = VTUtils::getOffset(vMethod);
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return ptr.get();
        }

        Destructible *getDtorMock() {
            auto offset = VTUtils::getDestructorOffset<C>();
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return ptr.get();
        }

        template<typename DataType, typename ... arglist>
        void stubDataMember(DataType C::*member, const arglist &... initargs) {
            DataType C::*theMember = (DataType C::*) member;
            C &mock = get();
            DataType *memberPtr = &(mock.*theMember);
            _members.push_back(
                    std::shared_ptr<DataMemeberWrapper < DataType, arglist...> >
                    {new DataMemeberWrapper < DataType, arglist...>(memberPtr,
                    initargs...)});
        }

        template<typename DataType>
        void getMethodMocks(std::vector<DataType> &into) const {
            for (std::shared_ptr<Destructible> ptr : _methodMocks) {
                DataType p = dynamic_cast<DataType>(ptr.get());
                if (p) {
                    into.push_back(p);
                }
            }
        }

        VirtualTable<C, baseclasses...> &getOriginalVT() {
            return _originalVt;
        }

        template<typename R, typename ... arglist>
        Finally createRaiiMethodSwapper(R(C::*vMethod)(arglist...)) {
            auto offset = VTUtils::getOffset(vMethod);
            auto fakeMethod = getFake().getVirtualTable().getMethod(offset);
            auto originalMethod = getOriginalVT().getMethod(offset);

            getFake().setMethod(offset, originalMethod);
            return Finally{[&, offset, fakeMethod](){
                getFake().setMethod(offset, fakeMethod);
            }};
        }

    private:

        template<typename DataType, typename ... arglist>
        class DataMemeberWrapper : public Destructible {
        private:
            DataType *dataMember;
        public:
            DataMemeberWrapper(DataType *dataMem, const arglist &... initargs) :
                    dataMember(dataMem) {
                new(dataMember) DataType{initargs ...};
            }

            ~DataMemeberWrapper() override
            {
                dataMember->~DataType();
            }
        };

        static_assert(sizeof(C) == sizeof(FakeObject<C, baseclasses...>), "This is a problem");

        C* _instancePtr = nullptr;
        VirtualTable<C, baseclasses...> _originalVt; // avoid delete!! this is the original!
        //
        std::vector<std::shared_ptr<Destructible>> _methodMocks;
        std::vector<std::shared_ptr<Destructible>> _members;
        std::vector<size_t> _offsets;
        InvocationHandlers _invocationHandlers;

        FakeObject<C, baseclasses...> &getFake() {
            return reinterpret_cast<FakeObject<C, baseclasses...> &>(*_instancePtr);
        }

        void bind(const MethodProxy &methodProxy, Destructible *invocationHandler) {
            getFake().setMethod(methodProxy.getOffset(), methodProxy.getProxy());
            _methodMocks[methodProxy.getOffset()].reset(invocationHandler);
            _offsets[methodProxy.getOffset()] = methodProxy.getId();
        }

        void bindDtor(const MethodProxy &methodProxy, Destructible *invocationHandler) {
            getFake().setDtor(methodProxy.getProxy());
            _methodMocks[methodProxy.getOffset()].reset(invocationHandler);
            _offsets[methodProxy.getOffset()] = methodProxy.getId();
        }

        template<typename DataType>
        DataType getMethodMock(unsigned int offset) {
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return dynamic_cast<DataType>(ptr.get());
        }

        template<typename BaseClass>
        void checkMultipleInheritance() {
            C *ptr = (C *) (unsigned int) 1;
            BaseClass *basePtr = ptr;
            int delta = (unsigned long) basePtr - (unsigned long) ptr;
            if (delta > 0) {
                // base class does not start on same position as derived class.
                // this is multiple inheritance.
                throw std::invalid_argument(std::string("multiple inheritance is not supported"));
            }
        }

        bool isBinded(unsigned int offset) {
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return ptr.get() != nullptr;
        }

    };
}
