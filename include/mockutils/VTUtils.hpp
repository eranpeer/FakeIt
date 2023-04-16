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
#include "mockutils/Macros.hpp"
#include "mockutils/VirtualOffestSelector.hpp"
#include "mockutils/union_cast.hpp"

namespace fakeit {
    class NoVirtualDtor : public std::runtime_error {
    public:
		NoVirtualDtor() :std::runtime_error("Can't mock the destructor. No virtual destructor was found") {}
    };

    class VTUtils {
    public:

#if defined(__GNUG__) && !defined(__clang__) && __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
        template<typename C, typename R, typename ... arglist>
        static unsigned int getOffset(R (C::*vMethod)(arglist...)) {
            auto sMethod = reinterpret_cast<unsigned int (VirtualOffsetSelector::*)(int)>(vMethod);
            VirtualOffsetSelector offsetSelctor;
            return (offsetSelctor.*sMethod)(0);
        }
#if defined(__GNUG__) && !defined(__clang__) && __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif

        template<typename C>
        FAKEIT_DISARM_UBSAN
        static typename std::enable_if<std::has_virtual_destructor<C>::value, unsigned int>::type
        getDestructorOffset() {
            VirtualOffsetSelector offsetSelctor;
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

            unsigned int vtSize = getOffset(&Derrived::endOfVt);
            return vtSize;
        }
    };


}
