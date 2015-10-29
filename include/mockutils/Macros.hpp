/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#if defined (__GNUG__) || _MSC_VER >= 1900
#define THROWS noexcept(false)
#define NO_THROWS noexcept(true)
#elif defined (_MSC_VER)
#define THROWS throw(...)
#define NO_THROWS
#endif
