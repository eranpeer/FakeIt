/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <functional>
#include <type_traits>
#include <vector>
#include <array>
#include <new>

#include "mockutils/VirtualTable.hpp"
#include "mockutils/union_cast.hpp"
#include "mockutils/MethodInvocationHandler.hpp"
#include "mockutils/VTUtils.hpp"
#include "mockutils/FakeObject.hpp"
#include "mockutils/MethodProxy.hpp"
#include "mockutils/MethodProxyCreator.hpp"

namespace fakeit {

    class InvocationHandlers : public InvocationHandlerCollection {
        std::vector<std::shared_ptr<Destructible>> &_methodMocks;
        std::vector<unsigned int> &_offsets;

        unsigned int getOffset(unsigned int id) const
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
        InvocationHandlers(
                std::vector<std::shared_ptr<Destructible>> &methodMocks,
                std::vector<unsigned int> &offsets) :
                _methodMocks(methodMocks), _offsets(offsets) {
        }

        Destructible *getInvocatoinHandlerPtrById(unsigned int id) override {
            unsigned int offset = getOffset(id);
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return ptr.get();
        }

    };

    template<typename C, typename ... baseclasses>
    struct DynamicProxy {

        static_assert(std::is_polymorphic<C>::value, "DynamicProxy requires a polymorphic type");

        DynamicProxy(C &inst) :
                instance(inst),
                originalVtHandle(VirtualTable<C, baseclasses...>::getVTable(instance).createHandle()),
                _methodMocks(VTUtils::getVTSize<C>()),
                _offsets(VTUtils::getVTSize<C>()),
                _invocationHandlers(_methodMocks, _offsets) {
            _cloneVt.copyFrom(originalVtHandle.restore());
            _cloneVt.setCookie(InvocationHandlerCollection::VT_COOKIE_INDEX, &_invocationHandlers);
            getFake().setVirtualTable(_cloneVt);
        }

        void detach() {
            getFake().setVirtualTable(originalVtHandle.restore());
        }

        ~DynamicProxy() {
            _cloneVt.dispose();
        }

        C &get() {
            return instance;
        }

        void Reset() {
			_methodMocks = {};
            _methodMocks.resize(VTUtils::getVTSize<C>());
            _members = {};
			_offsets = {};
            _offsets.resize(VTUtils::getVTSize<C>());
            _cloneVt.copyFrom(originalVtHandle.restore());
        }

		void Clear()
        {
        }

        template<int id, typename R, typename ... arglist>
        void stubMethod(R(C::*vMethod)(arglist...), MethodInvocationHandler<R, arglist...> *methodInvocationHandler) {
            auto offset = VTUtils::getOffset(vMethod);
            MethodProxyCreator<R, arglist...> creator;
            bind(creator.template createMethodProxy<id + 1>(offset), methodInvocationHandler);
        }

        void stubDtor(MethodInvocationHandler<void> *methodInvocationHandler) {
            auto offset = VTUtils::getDestructorOffset<C>();
            MethodProxyCreator<void> creator;
            bindDtor(creator.createMethodProxy<0>(offset), methodInvocationHandler);
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

        template<typename DATA_TYPE, typename ... arglist>
        void stubDataMember(DATA_TYPE C::*member, const arglist &... initargs) {
            DATA_TYPE C::*theMember = (DATA_TYPE C::*) member;
            C &mock = get();
            DATA_TYPE *memberPtr = &(mock.*theMember);
            _members.push_back(
                    std::shared_ptr<DataMemeberWrapper < DATA_TYPE, arglist...> >
                    {new DataMemeberWrapper < DATA_TYPE, arglist...>(memberPtr,
                    initargs...)});
        }

        template<typename DATA_TYPE>
        void getMethodMocks(std::vector<DATA_TYPE> &into) const {
            for (std::shared_ptr<Destructible> ptr : _methodMocks) {
                DATA_TYPE p = dynamic_cast<DATA_TYPE>(ptr.get());
                if (p) {
                    into.push_back(p);
                }
            }
        }

        VirtualTable<C, baseclasses...> &getOriginalVT() {
            VirtualTable<C, baseclasses...> &vt = originalVtHandle.restore();
            return vt;
        }

    private:

        template<typename DATA_TYPE, typename ... arglist>
        class DataMemeberWrapper : public Destructible {
        private:
            DATA_TYPE *dataMember;
        public:
            DataMemeberWrapper(DATA_TYPE *dataMem, const arglist &... initargs) :
                    dataMember(dataMem) {
                new(dataMember) DATA_TYPE{initargs ...};
            }

            ~DataMemeberWrapper() override
            {
                dataMember->~DATA_TYPE();
            }
        };

        static_assert(sizeof(C) == sizeof(FakeObject<C, baseclasses...>), "This is a problem");

        C &instance;
        typename VirtualTable<C, baseclasses...>::Handle originalVtHandle; // avoid delete!! this is the original!
        VirtualTable<C, baseclasses...> _cloneVt;
        //
        std::vector<std::shared_ptr<Destructible>> _methodMocks;
        std::vector<std::shared_ptr<Destructible>> _members;
        std::vector<unsigned int> _offsets;
        InvocationHandlers _invocationHandlers;

        FakeObject<C, baseclasses...> &getFake() {
            return reinterpret_cast<FakeObject<C, baseclasses...> &>(instance);
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

        template<typename DATA_TYPE>
        DATA_TYPE getMethodMock(unsigned int offset) {
            std::shared_ptr<Destructible> ptr = _methodMocks[offset];
            return dynamic_cast<DATA_TYPE>(ptr.get());
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
