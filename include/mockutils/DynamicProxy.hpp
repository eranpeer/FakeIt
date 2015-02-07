/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DynamicProxy_h__
#define DynamicProxy_h__

#include <functional>
#include <type_traits>
#include <vector>
#include <array>
#include <new>

#ifdef _MSC_VER
#include "mockutils/mscpp/VirtualTable.hpp"
#else
#include "mockutils/gcc/VirtualTable.hpp"
#endif

#include "mockutils/union_cast.hpp"
#include "mockutils/MethodInvocationHandler.hpp"
#include "mockutils/VTUtils.hpp"
#include "mockutils/FakeObject.hpp"
#include "mockutils/MethodProxy.hpp"
#include "MethodProxyCreator.hpp"

namespace fakeit {

    template<typename C, typename ... baseclasses>
    struct DynamicProxy {

        class InvocationHandlers : public InvocationHandlerCollection {
            std::array<std::shared_ptr<Destructable>, 50>& _methodMocks;
        public:
            InvocationHandlers(std::array<std::shared_ptr<Destructable>, 50>& methodMocks) : _methodMocks(methodMocks){
            }

            Destructable* getInvocatoinHandlerPtr(unsigned int offset) override {
                std::shared_ptr<Destructable> ptr = _methodMocks[offset];
                return ptr.get();
            }
        };

        static_assert(std::is_polymorphic<C>::value, "DynamicProxy requires a polymorphic type");

        DynamicProxy(C& instance) :
            instance(instance), originalVtHandle(VirtualTable<C, baseclasses...>::getVTable(instance).createHandle()),
            _methodMocks(),
            _invocationHandlers(_methodMocks)
        {
            _cloneVt.copyFrom(originalVtHandle.restore());
            _cloneVt.setCookie(InvocationHandlerCollection::VT_COOKIE_INDEX, &_invocationHandlers);
            getFake().setVirtualTable(_cloneVt);
        }

        void detach()
        {
            getFake().setVirtualTable(originalVtHandle.restore());
        }

        ~DynamicProxy() {
            _cloneVt.dispose();
        }

        C& get() {
            return instance;
        }

        void Reset() {
            _methodMocks = { {} };
            _members = {};
            _cloneVt.copyFrom(originalVtHandle.restore());
        }

        template<typename R, typename ... arglist>
        void stubMethod(R(C::*vMethod)(arglist...), MethodInvocationHandler<R, arglist...>* methodInvocationHandler) {
            auto offset = VTUtils::getOffset(vMethod);
            MethodProxyCreator<R, arglist...> creator;
            bindMethod(creator.createMethodProxy(offset), methodInvocationHandler);
        }

        void stubDtor(MethodInvocationHandler<void>* methodInvocationHandler) {
            auto offset = VTUtils::getDestructorOffset<C>();
            MethodProxyCreator<void> creator;
            bindDtor(creator.createMethodProxy(offset), methodInvocationHandler);
        }

        template<typename R, typename ... arglist>
        bool isMethodStubbed(R(C::*vMethod)(arglist...)) {
            unsigned int offset = VTUtils::getOffset(vMethod);
            return isBinded(offset);
        }

        bool isDtorStubbed() {
            unsigned int offset = VTUtils::getDestructorOffset<C>();
            return isBinded((unsigned int) offset);
        }

        template<typename R, typename ... arglist>
        Destructable * getMethodMock(R(C::*vMethod)(arglist...)) {
            auto offset = VTUtils::getOffset(vMethod);
            std::shared_ptr<Destructable> ptr = _methodMocks[offset];
            return ptr.get();
        }

        Destructable * getDtorMock() {
            auto offset = VTUtils::getDestructorOffset<C>();
            std::shared_ptr<Destructable> ptr = _methodMocks[offset];
            return ptr.get();
        }

        template<typename DATA_TYPE, typename ... arglist>
        void stubDataMember(DATA_TYPE C::*member, const arglist&... initargs) {
            DATA_TYPE C::*theMember = (DATA_TYPE C::*) member;
            C& mock = get();
            DATA_TYPE *memberPtr = &(mock.*theMember);
            _members.push_back(
                std::shared_ptr < DataMemeberWrapper<DATA_TYPE, arglist...> > {new DataMemeberWrapper<DATA_TYPE, arglist...>(memberPtr,
                initargs...)});
        }

        template<typename DATA_TYPE>
        void getMethodMocks(std::vector<DATA_TYPE>& into) const {
            for (std::shared_ptr<Destructable> ptr : _methodMocks) {
                DATA_TYPE p = dynamic_cast<DATA_TYPE>(ptr.get());
                if (p) {
                    into.push_back(p);
                }
            }
        }

        VirtualTable<C, baseclasses...>& getOriginalVT() {
            VirtualTable<C, baseclasses...>& vt = originalVtHandle.restore();
            return vt;
        }

    private:

        template<typename DATA_TYPE, typename ... arglist>
        class DataMemeberWrapper : public Destructable {
        private:
            DATA_TYPE *dataMember;
        public:
            DataMemeberWrapper(DATA_TYPE *dataMember, const arglist&... initargs) :
                dataMember(dataMember) {
                new (dataMember) DATA_TYPE{ initargs ... };
            }
            ~DataMemeberWrapper() {
                dataMember->~DATA_TYPE();
            }
        };

        static_assert(sizeof(C) == sizeof(FakeObject<C, baseclasses...>), "This is a problem");

        C& instance;
        typename VirtualTable<C, baseclasses...>::Handle originalVtHandle; // avoid delete!! this is the original!
        VirtualTable<C, baseclasses...> _cloneVt;//
        std::array<std::shared_ptr<Destructable>, 50> _methodMocks;
        std::vector<std::shared_ptr<Destructable>> _members;
        InvocationHandlers _invocationHandlers;

        FakeObject<C, baseclasses...>& getFake() {
            return reinterpret_cast<FakeObject<C, baseclasses...>&>(instance);
        }

        template<typename R, typename ... arglist>
        void bindMethod(const MethodProxy &methodProxy, MethodInvocationHandler<R, arglist...> *invocationHandler) {
            auto offset = methodProxy.getOffset();
            getFake().setMethod(offset, methodProxy.getProxy());
            Destructable * destructable = invocationHandler;
            _methodMocks[offset].reset(destructable);
        }

        void bindDtor(const MethodProxy &methodProxy, MethodInvocationHandler<void> *invocationHandler) {
            auto offset = methodProxy.getOffset();
            getFake().setDtor(methodProxy.getProxy());
            Destructable * destructable = invocationHandler;
            _methodMocks[offset].reset(destructable);
        }

        template<typename DATA_TYPE>
        DATA_TYPE getMethodMock(unsigned int offset) {
            std::shared_ptr<Destructable> ptr = _methodMocks[offset];
            return dynamic_cast<DATA_TYPE>(ptr.get());
        }

        template<typename BaseClass>
        void checkMultipleInheritance() {
            C* ptr = (C*) (unsigned int) 1;
            BaseClass* basePtr = ptr;
            int delta = (unsigned long) basePtr - (unsigned long) ptr;
            if (delta > 0) {
                // base class does not start on same position as derived class.
                // this is multiple inheritance.
                throw std::invalid_argument(std::string("multiple inheritance is not supported"));
            }
        }

        bool isBinded(unsigned int offset) {
            std::shared_ptr<Destructable> ptr = _methodMocks[offset];
            return ptr.get() != nullptr;
        }

    };
}
#endif // DynamicProxy_h__
