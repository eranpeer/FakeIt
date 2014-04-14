/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef Macros_h__
#define Macros_h__

#if defined (_WIN32)
#define THROWS throw(...)
#elif defined (__GNUG__)
#define THROWS noexcept(false)
#endif

#endif
