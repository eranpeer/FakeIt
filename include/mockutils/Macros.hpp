/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#if defined (__GNUG__)
#define THROWS noexcept(false)
#elif defined (_MSC_VER)
#define THROWS throw(...)
#endif
