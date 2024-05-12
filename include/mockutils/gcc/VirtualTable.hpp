/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <utility>

#ifndef __clang__

#include "mockutils/gcc/is_simple_inheritance_layout.hpp"

#endif

#include "mockutils/Macros.hpp"

namespace fakeit {

    struct VirtualTableBase {

        static VirtualTableBase &getVTable(void *instance) {
            fakeit::VirtualTableBase *vt = (fakeit::VirtualTableBase *) (instance);
            return *vt;
        }

        VirtualTableBase(void **firstMethod) : _firstMethod(firstMethod) { }

        VirtualTableBase(const VirtualTableBase&) = delete;
        VirtualTableBase(VirtualTableBase&& other) FAKEIT_NO_THROWS {
            std::swap(_firstMethod, other._firstMethod);
        }

        VirtualTableBase& operator=(const VirtualTableBase&) = delete;
        VirtualTableBase& operator=(VirtualTableBase&& other) FAKEIT_NO_THROWS {
            std::swap(_firstMethod, other._firstMethod);
            return *this;
        }

        ~VirtualTableBase() {
            if (_firstMethod != nullptr) {
                _firstMethod--; // type_info
                _firstMethod--; // top_offset
                _firstMethod -= numOfCookies; // skip cookies
                delete[] _firstMethod;
            }
        }

        void *getCookie(int index) {
            return _firstMethod[-3 - index];
        }

        void setCookie(int index, void *value) {
            _firstMethod[-3 - index] = value;
        }

        void *getMethod(unsigned int index) const {
            return _firstMethod[index];
        }

        void setMethod(unsigned int index, void *method) {
            _firstMethod[index] = method;
        }

    protected:
        static const unsigned int numOfCookies = 2;
        void **_firstMethod = nullptr;
    };

    template<class C, class ... baseclasses>
    struct VirtualTable : public VirtualTableBase {

#ifndef __clang__
        static_assert(is_simple_inheritance_layout<C>::value, "Can't mock a type with multiple inheritance");
#endif

        static VirtualTable<C, baseclasses...> &getVTable(C &instance) {
            fakeit::VirtualTable<C, baseclasses...> *vt = (fakeit::VirtualTable<C, baseclasses...> *) (&instance);
            return *vt;
        }

        void copyFrom(VirtualTable<C, baseclasses...> &from) {
            unsigned int size = VTUtils::getVTSize<C>();
            //firstMethod[-1] = from.firstMethod[-1]; // copy type_info
            for (size_t i = 0; i < size; ++i) {
                _firstMethod[i] = from.getMethod(i);
            }
        }

        VirtualTable() :
                VirtualTable(buildVTArray()) {
        }

        unsigned int dtor(int) {
            C *c = (C *) this;
            C &cRef = *c;
            auto& vt = VirtualTable<C, baseclasses...>::getVTable(cRef);
            unsigned int index = VTUtils::getDestructorOffset<C>();
            void *dtorPtr = vt.getMethod(index);
            void(*method)(C *) = union_cast<void (*)(C *)>(dtorPtr);
            method(c);
            return 0;
        }

        void setDtor(void *method) {
            unsigned int index = VTUtils::getDestructorOffset<C>();
            void *dtorPtr = union_cast<void *>(&VirtualTable<C, baseclasses...>::dtor);
            // replace the non deleting destructor.
            // for example (c1->~C()) calls the non deleting dtor only
            _firstMethod[index] = method;
            // replace the deleting destructor with a method that calls the non deleting one
            _firstMethod[index + 1] = dtorPtr;
        }

        unsigned int getSize() {
            return VTUtils::getVTSize<C>();
        }

        void initAll(void *value) {
            unsigned int size = getSize();
            for (unsigned int i = 0; i < size; i++) {
                setMethod(i, value);
            }
        }

        const std::type_info *getTypeId() {
            return (const std::type_info *) (_firstMethod[-1]);
        }

    private:
        static void **buildVTArray() {
            unsigned int size = VTUtils::getVTSize<C>();
            auto array = new void *[size + 2 + numOfCookies]{};
            array += numOfCookies; // skip cookies
            array++; // skip top_offset
            array[0] = const_cast<std::type_info *>(&typeid(C)); // type_info
            array++; // skip type_info ptr
            return array;
        }

        VirtualTable(void **firstMethod) : VirtualTableBase(firstMethod) {
        }

    };
}
