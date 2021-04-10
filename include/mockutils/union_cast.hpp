/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#if defined(__GNUG__) && !defined(__clang__)
#define FAKEIT_NO_DEVIRTUALIZE_ATTR [[gnu::optimize("no-devirtualize")]]
#else
#define FAKEIT_NO_DEVIRTUALIZE_ATTR
#endif 

namespace fakeit {

    template<typename TARGET, typename SOURCE>
    FAKEIT_NO_DEVIRTUALIZE_ATTR
    TARGET union_cast(SOURCE source) {
        //static_assert(sizeof(TARGET) == sizeof(SOURCE), "can't convert");
        union {
            SOURCE source;
            TARGET target;
        } u;
        u.source = source;
        return u.target;
    }

}
