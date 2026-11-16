/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifdef _MSC_VER

#include "tpunit++.hpp"
#include "fakeit.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

using namespace fakeit;

struct MscSpyingTests : tpunit::TestFixture
{

    MscSpyingTests() :
        TestFixture(
            TEST(MscSpyingTests::spy_does_not_read_before_virtual_table_of_spied_object)
        )
    {
    }

    struct SomeClass
    {
        virtual ~SomeClass() = default;
        virtual int func() { return 1; }
    };

    // A spied object's virtual table comes from the compiler, so FakeIt must not read anything before it. Put a copy
    // of the table right after an inaccessible page so that any such read crashes.
    void spy_does_not_read_before_virtual_table_of_spied_object()
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        char *pages = static_cast<char *>(VirtualAlloc(nullptr, 2 * systemInfo.dwPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
        DWORD oldProtection;
        BOOL isGuarded = VirtualProtect(pages, systemInfo.dwPageSize, PAGE_NOACCESS, &oldProtection);
        ASSERT_TRUE(isGuarded);

        SomeClass obj;
        void **&vptr = *reinterpret_cast<void ***>(&obj);
        void **originalTable = vptr;
        void **table = reinterpret_cast<void **>(pages + systemInfo.dwPageSize);
        for (unsigned int i = 0; i < VTUtils::getVTSize<SomeClass>(); i++)
            table[i] = originalTable[i];
        vptr = table;
        {
            Mock<SomeClass> spy(obj);
            Spy(Method(spy, func));
            ASSERT_EQUAL(1, spy.get().func());
            spy.Reset();
        }
        vptr = originalTable;
        VirtualFree(pages, 0, MEM_RELEASE);
    }

} __MscSpyingTests;

#endif
