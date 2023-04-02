#pragma once
#include <utility>

#include "mockutils/type_utils.hpp"
#include "mockutils/VirtualTable.hpp"
#include "mockutils/MethodProxy.hpp"
#include "mockutils/Destructible.hpp"

namespace fakeit {

    struct InvocationHandlerCollection {
        static const unsigned int VtCookieIndex = 0;

        virtual Destructible *getInvocatoinHandlerPtrById(unsigned int index) = 0;

        static InvocationHandlerCollection *getInvocationHandlerCollection(void *instance) {
            VirtualTableBase &vt = VirtualTableBase::getVTable(instance);
            InvocationHandlerCollection *invocationHandlerCollection = (InvocationHandlerCollection *) vt.getCookie(
                    InvocationHandlerCollection::VtCookieIndex);
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

        template<unsigned int id>
        MethodProxy createMethodProxyStatic(unsigned int offset) {
            return MethodProxy(id, offset, union_cast<void *>(&MethodProxyCreator::methodProxyXStatic < id > ));
        }

    protected:

        R methodProxy(unsigned int id, const typename fakeit::production_arg<arglist>::type... args) {
            InvocationHandlerCollection *invocationHandlerCollection = InvocationHandlerCollection::getInvocationHandlerCollection(
                    this);
            MethodInvocationHandler<R, arglist...> *invocationHandler =
                    (MethodInvocationHandler<R, arglist...> *) invocationHandlerCollection->getInvocatoinHandlerPtrById(
                            id);
            return invocationHandler->handleMethodInvocation(std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }

        template<int id>
        R methodProxyX(arglist ... args) {
            return methodProxy(id, std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }

        static R methodProxyStatic(void* instance, unsigned int id, const typename fakeit::production_arg<arglist>::type... args) {
            InvocationHandlerCollection *invocationHandlerCollection = InvocationHandlerCollection::getInvocationHandlerCollection(
                instance);
            MethodInvocationHandler<R, arglist...> *invocationHandler =
                (MethodInvocationHandler<R, arglist...> *) invocationHandlerCollection->getInvocatoinHandlerPtrById(
                    id);
            return invocationHandler->handleMethodInvocation(std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }

        template<int id>
        static R methodProxyXStatic(void* instance, arglist ... args) {
            return methodProxyStatic(instance, id, std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }
    };
}
