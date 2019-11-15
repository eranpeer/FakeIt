/*
 * VTUtils.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jun 3, 2014
 */
#pragma once

#include <functional>
#include <type_traits>
#include "mockutils/VirtualOffestSelector.hpp"
#include "mockutils/union_cast.hpp"

namespace fakeit {
    class NoVirtualDtor : public std::runtime_error {
    public:
		NoVirtualDtor() :std::runtime_error("Can't mock the destructor. No virtual destructor was found") {}
    };

    class VTUtils {
    public:

        template<typename C, typename R,  typename ... arglist>
        static unsigned int getOffset(FuncWithConvention<C, R, Thiscall, arglist...> vMethod) {
            auto sMethod = reinterpret_cast<unsigned int (VirtualOffsetSelector<Thiscall>::*)(int)>(vMethod._vMethod);
            VirtualOffsetSelector<Thiscall> offsetSelctor;
            return (offsetSelctor.*sMethod)(0);
        }

        template<typename C, typename R,  typename ... arglist>
        static unsigned int getOffset(FuncWithConvention<C, R, Cdecl, arglist...> vMethod) {
            auto sMethod = reinterpret_cast<unsigned int (VirtualOffsetSelector<Cdecl>::*)(int)>(vMethod._vMethod);
            VirtualOffsetSelector<Cdecl> offsetSelctor;
            return (offsetSelctor.*sMethod)(0);
        }

        template<typename C, typename R, typename ... arglist>
        static unsigned int getOffset(FuncWithConvention<C, R, Stdcall, arglist...> vMethod) {
            auto sMethod = reinterpret_cast<unsigned int (__stdcall VirtualOffsetSelector<Stdcall>::*)(int)>(vMethod._vMethod);
            VirtualOffsetSelector<Stdcall> offsetSelctor;
            return (offsetSelctor.*sMethod)(0);
        }

        template<typename C>
        static typename std::enable_if<std::has_virtual_destructor<C>::value, unsigned int>::type
        getDestructorOffset() {
			// Destructors always use the default convention (at least when dealing with COM, which is all we care about).
            VirtualOffsetSelector<ConventionHelper::DefaultConvention> offsetSelctor;
            union_cast<C *>(&offsetSelctor)->~C();
            return offsetSelctor.offset;
        }

        template<typename C>
        static typename std::enable_if<!std::has_virtual_destructor<C>::value, unsigned int>::type
        getDestructorOffset() {
            throw NoVirtualDtor();
        }

		template<typename C>
		static typename std::enable_if<std::has_virtual_destructor<C>::value, bool>::type
			hasVirtualDestructor() {
			return true;
		}

		template<typename C>
		static typename std::enable_if<!std::has_virtual_destructor<C>::value, bool>::type
			hasVirtualDestructor() {
			return false;
		}

        template<typename C>
        static unsigned int getVTSize() {
            struct Derrived : public C {
                virtual void endOfVt() {
                }
            };

            unsigned int vtSize = getOffset( ConventionHelper::Wrap( &Derrived::endOfVt ) );
            return vtSize;
        }
    };


}
