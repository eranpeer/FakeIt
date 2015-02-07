#pragma once

#ifdef _MSC_VER
#include "mockutils/mscpp/VirtualTable.hpp"
#else
#include "mockutils/gcc/VirtualTable.hpp"
#endif
#include "mockutils/MethodProxy.hpp"
#include "mockutils/Destructable.hpp"

namespace fakeit {

    struct InvocationHandlerCollection {
        static const unsigned int VT_COOKIE_INDEX = 0;
        virtual Destructable* getInvocatoinHandlerPtr(unsigned int index) = 0;
    };

    static InvocationHandlerCollection * getInvocationHandlerCollection(void * instance) {
        VirtualTableBase & vt = VirtualTableBase::getVTable(instance);
        InvocationHandlerCollection * invocationHandlerCollection = (InvocationHandlerCollection*) 
            vt.getCookie(InvocationHandlerCollection::VT_COOKIE_INDEX);
        return invocationHandlerCollection;
    }

    template<typename R, typename ... arglist>
    class MethodProxyCreator {
    protected:

        MethodProxy newMethodProxy(unsigned int offset, R(MethodProxyCreator::*vMethod)(arglist...)) {
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

        using MethodProxyPtrType = R(MethodProxyCreator::*)(arglist...);
        MethodProxyPtrType getMethodProxyPtr(unsigned int offset){
            R(MethodProxyCreator::*vMethod)(arglist...) = 0;
            switch (offset) {
            case 0:
                vMethod = &MethodProxyCreator::methodProxyX < 0 > ;
                break;
            case 1:
                vMethod = &MethodProxyCreator::methodProxyX < 1 > ;
                break;
            case 2:
                vMethod = &MethodProxyCreator::methodProxyX < 2 > ;
                break;
            case 3:
                vMethod = &MethodProxyCreator::methodProxyX < 3 > ;
                break;
            case 4:
                vMethod = &MethodProxyCreator::methodProxyX < 4 > ;
                break;
            case 5:
                vMethod = &MethodProxyCreator::methodProxyX < 5 > ;
                break;
            case 6:
                vMethod = &MethodProxyCreator::methodProxyX < 6 > ;
                break;
            case 7:
                vMethod = &MethodProxyCreator::methodProxyX < 7 > ;
                break;
            case 8:
                vMethod = &MethodProxyCreator::methodProxyX < 8 > ;
                break;
            case 9:
                vMethod = &MethodProxyCreator::methodProxyX < 9 > ;
                break;
            case 10:
                vMethod = &MethodProxyCreator::methodProxyX < 10 > ;
                break;
            case 11:
                vMethod = &MethodProxyCreator::methodProxyX < 11 > ;
                break;
            case 12:
                vMethod = &MethodProxyCreator::methodProxyX < 12 > ;
                break;
            case 13:
                vMethod = &MethodProxyCreator::methodProxyX < 13 > ;
                break;
            case 14:
                vMethod = &MethodProxyCreator::methodProxyX < 14 > ;
                break;
            case 15:
                vMethod = &MethodProxyCreator::methodProxyX < 15 > ;
                break;
            case 16:
                vMethod = &MethodProxyCreator::methodProxyX < 16 > ;
                break;
            case 17:
                vMethod = &MethodProxyCreator::methodProxyX < 17 > ;
                break;
            case 18:
                vMethod = &MethodProxyCreator::methodProxyX < 18 > ;
                break;
            case 19:
                vMethod = &MethodProxyCreator::methodProxyX < 19 > ;
                break;
            case 20:
                vMethod = &MethodProxyCreator::methodProxyX < 20 > ;
                break;
            case 21:
                vMethod = &MethodProxyCreator::methodProxyX < 21 > ;
                break;
            case 22:
                vMethod = &MethodProxyCreator::methodProxyX < 22 > ;
                break;
            case 23:
                vMethod = &MethodProxyCreator::methodProxyX < 23 > ;
                break;
            case 24:
                vMethod = &MethodProxyCreator::methodProxyX < 24 > ;
                break;
            case 25:
                vMethod = &MethodProxyCreator::methodProxyX < 25 > ;
                break;
            case 26:
                vMethod = &MethodProxyCreator::methodProxyX < 26 > ;
                break;
            case 27:
                vMethod = &MethodProxyCreator::methodProxyX < 27 > ;
                break;
            case 28:
                vMethod = &MethodProxyCreator::methodProxyX < 28 > ;
                break;
            case 29:
                vMethod = &MethodProxyCreator::methodProxyX < 29 > ;
                break;
            case 30:
                vMethod = &MethodProxyCreator::methodProxyX < 30 > ;
                break;
            case 31:
                vMethod = &MethodProxyCreator::methodProxyX < 31 > ;
                break;
            case 32:
                vMethod = &MethodProxyCreator::methodProxyX < 32 > ;
                break;
            case 33:
                vMethod = &MethodProxyCreator::methodProxyX < 33 > ;
                break;
            case 34:
                vMethod = &MethodProxyCreator::methodProxyX < 34 > ;
                break;
            case 35:
                vMethod = &MethodProxyCreator::methodProxyX < 35 > ;
                break;
            case 36:
                vMethod = &MethodProxyCreator::methodProxyX < 36 > ;
                break;
            case 37:
                vMethod = &MethodProxyCreator::methodProxyX < 37 > ;
                break;
            case 38:
                vMethod = &MethodProxyCreator::methodProxyX < 38 > ;
                break;
            case 39:
                vMethod = &MethodProxyCreator::methodProxyX < 39 > ;
                break;
            case 40:
                vMethod = &MethodProxyCreator::methodProxyX < 40 > ;
                break;
            case 41:
                vMethod = &MethodProxyCreator::methodProxyX < 41 > ;
                break;
            case 42:
                vMethod = &MethodProxyCreator::methodProxyX < 42 > ;
                break;
            case 43:
                vMethod = &MethodProxyCreator::methodProxyX < 43 > ;
                break;
            case 44:
                vMethod = &MethodProxyCreator::methodProxyX < 44 > ;
                break;
            case 45:
                vMethod = &MethodProxyCreator::methodProxyX < 45 > ;
                break;
            case 46:
                vMethod = &MethodProxyCreator::methodProxyX < 46 > ;
                break;
            case 47:
                vMethod = &MethodProxyCreator::methodProxyX < 47 > ;
                break;
            case 48:
                vMethod = &MethodProxyCreator::methodProxyX < 48 > ;
                break;
            case 49:
                vMethod = &MethodProxyCreator::methodProxyX < 49 > ;
                break;
            }
            return vMethod;
        }
    };
}

