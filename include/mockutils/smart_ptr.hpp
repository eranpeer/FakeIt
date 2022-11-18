/*
 * smart_ptr.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jul 17, 2014
 */
#pragma once

#include <exception>
#include "mockutils/Macros.hpp"

namespace fakeit {

    class RefCount {
    private:
        int count;

    public:
        void AddRef() {
            count++;
        }

        int Release() {
            return --count;
        }
    };

    template<typename T>
    class smart_ptr {
    private:
        T *pData;
        RefCount *reference;

    public:
        smart_ptr() : pData(0), reference(0) {
            reference = new RefCount();
            reference->AddRef();
        }

        smart_ptr(T *pValue) : pData(pValue), reference(0) {
            reference = new RefCount();
            reference->AddRef();
        }

        smart_ptr(const smart_ptr<T> &sp) : pData(sp.pData), reference(sp.reference) {
            reference->AddRef();
        }

        ~smart_ptr() FAKEIT_THROWS {
            if (reference->Release() == 0) {
                delete reference;
                delete pData;
            }
        }

        T &operator*() {
            return *pData;
        }

        T *operator->() {
            return pData;
        }

        smart_ptr<T> &operator=(const smart_ptr<T> &sp) {
            if (this != &sp) {
                // Decrement the old reference count
                // if reference become zero delete the old data
                if (reference->Release() == 0) {
                    delete reference;
                    delete pData;
                }

                // Copy the data and reference pointer
                // and increment the reference count
                pData = sp.pData;
                reference = sp.reference;
                reference->AddRef();
            }
            return *this;
        }
    };

}
