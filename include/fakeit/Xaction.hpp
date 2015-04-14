/*
 * Xaction.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 20, 2014
 */
#pragma once

namespace fakeit {

    struct Xaction {
        virtual void commit() = 0;
    };
}
