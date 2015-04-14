#pragma once
/*
 * to_string.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Sep 10, 2014
 */

#include <string>
#include <sstream>
#include <iomanip>

namespace fakeit {

    template<typename T>
    static std::string to_string(const T &n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }

}
