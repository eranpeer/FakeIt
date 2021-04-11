/*
 * FakeObject.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Jun 8, 2014
 */
#pragma once

#include <new>

#include "mockutils/VirtualTable.hpp"

namespace fakeit
{
    namespace details
    {
        template <int instanceAreaSize, typename C, typename... BaseClasses>
        class FakeObjectImpl
        {
        public:
            void initializeDataMembersArea()
            {
                for (size_t i = 0; i < instanceAreaSize; ++i)
                {
                    instanceArea[i] = (char) 0;
                }
            }

        protected:
            VirtualTable<C, BaseClasses...> vtable;
            char instanceArea[instanceAreaSize];
        };

        template <typename C, typename... BaseClasses>
        class FakeObjectImpl<0, C, BaseClasses...>
        {
        public:
            void initializeDataMembersArea()
            {}

        protected:
            VirtualTable<C, BaseClasses...> vtable;
        };
    } // namespace details

    template <typename C, typename... BaseClasses>
    class FakeObject
        : public details::FakeObjectImpl<sizeof(C) - sizeof(VirtualTable<C, BaseClasses...>), C, BaseClasses...>
    {
        FakeObject(FakeObject const&) = delete;            // undefined
        FakeObject& operator=(FakeObject const&) = delete; // undefined

    public:
        FakeObject()
        {
            this->initializeDataMembersArea();
        }

        ~FakeObject()
        {
            this->vtable.dispose();
        }

        void setMethod(unsigned int index, void* method)
        {
            this->vtable.setMethod(index, method);
        }

        VirtualTable<C, BaseClasses...>& getVirtualTable()
        {
            return this->vtable;
        }

        void setVirtualTable(VirtualTable<C, BaseClasses...>& t)
        {
            this->vtable = t;
        }

        void setDtor(void* dtor)
        {
            this->vtable.setDtor(dtor);
        }
    };
} // namespace fakeit
