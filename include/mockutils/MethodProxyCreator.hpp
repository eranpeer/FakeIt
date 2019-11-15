#pragma once
#include <utility>

#include "mockutils/type_utils.hpp"
#include "mockutils/VirtualTable.hpp"
#include "mockutils/MethodProxy.hpp"
#include "mockutils/Destructible.hpp"

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

	// Shared code for all specializations of MethodProxyCreator.
    template<typename R, typename ... arglist>
	class MethodProxyCreatorBase{

	public:

        template<unsigned int id, typename CONVENTION>
        MethodProxy createMethodProxy(unsigned int offset) {
            return MethodProxy(id, offset, union_cast<void *>
				(&MethodProxyCreator<R, CONVENTION, arglist...>::methodProxyX < id > ));
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
	};

	// Base case for method proxy creator specialization.
    template<typename R, typename CONVENTION, typename ... arglist>
	class MethodProxyCreator {};

	// Method proxies for __thiscall:s.
	template<typename R, typename ... arglist>
    class MethodProxyCreator< R, Thiscall, arglist...>: public MethodProxyCreatorBase< R, arglist...> {

    public:

        template<int id>
        R __thiscall methodProxyX(arglist ... args) {
            return methodProxy(id, std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }
    };

	// Method proxies for __stdcall:s.
	template<typename R, typename ... arglist>
    class MethodProxyCreator< R, Stdcall, arglist...>: public MethodProxyCreatorBase< R, arglist...> {

    public:

        template<int id>
        R __stdcall methodProxyX(arglist ... args) {
            return methodProxy(id, std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }
    };

	// Method proxies for __cdecl:s.
	template<typename R, typename ... arglist>
    class MethodProxyCreator< R, Cdecl, arglist...>: public MethodProxyCreatorBase< R, arglist...> {

    public:

        template<int id>
        R __cdecl methodProxyX(arglist ... args) {
            return methodProxy(id, std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);
        }
    };
}
