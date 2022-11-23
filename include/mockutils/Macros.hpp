/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#if defined (__GNUG__) || _MSC_VER >= 1900
#define FAKEIT_THROWS noexcept(false)
#define FAKEIT_NO_THROWS noexcept(true)
#elif defined (_MSC_VER)
#define FAKEIT_THROWS throw(...)
#define FAKEIT_NO_THROWS
#endif
