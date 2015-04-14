#pragma once

#ifdef _MSC_VER
#include "mockutils/mscpp/VirtualTable.hpp"
#else

#include "mockutils/gcc/VirtualTable.hpp"

#endif

#include "mockutils/MethodProxy.hpp"
#include "Destructible.hpp"

namespace fakeit {

    struct InvocationHandlerCollection {
        static const unsigned int VT_COOKIE_INDEX = 0;

        virtual Destructible *getInvocatoinHandlerPtrById(unsigned int index) = 0;

        static InvocationHandlerCollection *getInvocationHandlerCollection(void *instance) {
            VirtualTableBase &vt = VirtualTableBase::getVTable(instance);
            InvocationHandlerCollection *invocationHandlerCollection = (InvocationHandlerCollection *) vt.getCookie(
                    InvocationHandlerCollection::VT_COOKIE_INDEX);
            return invocationHandlerCollection;
        }
    };


    template<typename R, typename ... arglist>
    class MethodProxyCreator {

        //using MethodProxyPtrType = R(MethodProxyCreator::*)(arglist...);

    public:

        template<unsigned int id>
        MethodProxy createMethodProxy(unsigned int offset) {
            return MethodProxy(id, offset, union_cast<void *>(&MethodProxyCreator::methodProxyX < id > ));
        }

    protected:

        R methodProxy(unsigned int id, arglist &... args) {
            InvocationHandlerCollection *invocationHandlerCollection = InvocationHandlerCollection::getInvocationHandlerCollection(
                    this);
            MethodInvocationHandler<R, arglist...> *invocationHandler =
                    (MethodInvocationHandler<R, arglist...> *) invocationHandlerCollection->getInvocatoinHandlerPtrById(
                            id);
            return invocationHandler->handleMethodInvocation(args...);
        }

        template<int id>
        R methodProxyX(arglist ... args) {
            return methodProxy(id, args...);
        }
    };
}
