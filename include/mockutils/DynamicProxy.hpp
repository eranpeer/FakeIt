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

namespace fakeit {

    struct InvocationHandlerCollection {
        virtual Destructable* getInvocatoinHandlerPtr(unsigned int index) = 0;
    };

    static InvocationHandlerCollection * getInvocationHandlerCollection(void * instance) {
        VirtualTableBase & vt = VirtualTableBase::getVTable(instance);
        InvocationHandlerCollection * invocationHandlerCollection = (InvocationHandlerCollection*) vt.getCookie(0);
        return invocationHandlerCollection;
    }

    struct MethodProxy {

        MethodProxy(unsigned int offset, void * vMethod) :
                _vMethod(vMethod), _offset(offset) {
        }

        unsigned int getOffset() const {
            return _offset;
        }

        void * getProxy() const {
            return union_cast<void *>(_vMethod);
        }

    private:
        void * _vMethod;
        unsigned int _offset;
    };

    template<typename R, typename ... arglist>
    class MethodProxyCreatorBase{
    protected:

        MethodProxy newMethodProxy(unsigned int offset, R(MethodProxyCreatorBase::*vMethod)(arglist...)) {
            return MethodProxy(offset, union_cast<void *>(vMethod));
        }

    public:

        MethodProxy createMethodProxy(unsigned int offset) {
            return newMethodProxy(offset, getMethodProxyPtr(offset));
        }

    protected:

        R methodProxy(unsigned int index, arglist& ... args) {
            InvocationHandlerCollection *invocationHandlerCollection = getInvocationHandlerCollection(this);
            MethodInvocationHandler<R, arglist...> *invocationHandler =
                (MethodInvocationHandler<R, arglist...> *) invocationHandlerCollection->getInvocatoinHandlerPtr(index);
            return invocationHandler->handleMethodInvocation(args...);
        }

        template<int offset>
        R methodProxyX(arglist ... args) {
            return methodProxy(offset, args...);
        }

        using MethodProxyPtrType = R(MethodProxyCreatorBase::*)(arglist...);
        MethodProxyPtrType getMethodProxyPtr(unsigned int offset){
            R(MethodProxyCreatorBase::*vMethod)(arglist...) = 0;
            switch (offset) {
            case 0:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 0 > ;
                break;
            case 1:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 1 > ;
                break;
            case 2:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 2 > ;
                break;
            case 3:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 3 > ;
                break;
            case 4:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 4 > ;
                break;
            case 5:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 5 > ;
                break;
            case 6:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 6 > ;
                break;
            case 7:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 7 > ;
                break;
            case 8:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 8 > ;
                break;
            case 9:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 9 > ;
                break;
            case 10:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 10 > ;
                break;
            case 11:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 11 > ;
                break;
            case 12:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 12 > ;
                break;
            case 13:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 13 > ;
                break;
            case 14:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 14 > ;
                break;
            case 15:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 15 > ;
                break;
            case 16:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 16 > ;
                break;
            case 17:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 17 > ;
                break;
            case 18:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 18 > ;
                break;
            case 19:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 19 > ;
                break;
            case 20:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 20 > ;
                break;
            case 21:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 21 > ;
                break;
            case 22:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 22 > ;
                break;
            case 23:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 23 > ;
                break;
            case 24:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 24 > ;
                break;
            case 25:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 25 > ;
                break;
            case 26:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 26 > ;
                break;
            case 27:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 27 > ;
                break;
            case 28:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 28 > ;
                break;
            case 29:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 29 > ;
                break;
            case 30:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 30 > ;
                break;
            case 31:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 31 > ;
                break;
            case 32:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 32 > ;
                break;
            case 33:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 33 > ;
                break;
            case 34:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 34 > ;
                break;
            case 35:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 35 > ;
                break;
            case 36:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 36 > ;
                break;
            case 37:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 37 > ;
                break;
            case 38:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 38 > ;
                break;
            case 39:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 39 > ;
                break;
            case 40:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 40 > ;
                break;
            case 41:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 41 > ;
                break;
            case 42:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 42 > ;
                break;
            case 43:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 43 > ;
                break;
            case 44:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 44 > ;
                break;
            case 45:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 45 > ;
                break;
            case 46:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 46 > ;
                break;
            case 47:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 47 > ;
                break;
            case 48:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 48 > ;
                break;
            case 49:
                vMethod = &MethodProxyCreatorBase::methodProxyX < 49 > ;
                break;
            }
            return vMethod;
        }
    };


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
            _cloneVt.setCookie(0, &_invocationHandlers);
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
            MethodProxyCreator<R, arglist...> creator;
            bindMethod(creator.createMethodProxy(vMethod), methodInvocationHandler);
        }

        void stubDtor(MethodInvocationHandler<void>* methodInvocationHandler) {
            MethodProxyCreator<void> creator;
            bindDtor(creator.createDtorProxy(), methodInvocationHandler);
        }

        template<typename R, typename ... arglist>
        bool isMethodStubbed(R(C::*vMethod)(arglist...)) {
            unsigned int offset = MethodProxyCreator<R, arglist...>::getOffset(vMethod);
            return isBinded(offset);
        }

        bool isDtorStubbed() {
            unsigned int offset = VTUtils::getDestructorOffset<C>();
            return isBinded((unsigned int) offset);
        }

        template<typename R, typename ... arglist>
        Destructable * getMethodMock(R(C::*vMethod)(arglist...)) {
            auto offset = MethodProxyCreator<R, arglist...>::getOffset(vMethod);
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

        template<typename R, typename ... arglist>
        class MethodProxyCreator : public MethodProxyCreatorBase < R, arglist... > {
        public:

            MethodProxy createMethodProxy(R(C::*vMethod)(arglist...)) {
                auto offset = getOffset(vMethod);
                return MethodProxyCreatorBase < R, arglist... >::createMethodProxy(offset);
            }

            MethodProxy createDtorProxy() {
                auto offset = VTUtils::getDestructorOffset<C>();
                return MethodProxyCreatorBase < R, arglist... >::createMethodProxy(offset);
            }

            static unsigned int getOffset(R(C::*vMethod)(arglist...)) {
                return VTUtils::getOffset(vMethod);
            }

        };

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
