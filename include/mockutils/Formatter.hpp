/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <ostream>
#include <type_traits>
#include <string>
#include "mockutils/to_string.hpp"

namespace fakeit {

    template<class T>
    struct Formatter {
        static std::string format(const T &val) {
            if (std::is_const<T>::value)
                return Formatter<typename std::remove_const<T>::type>::format(val);
            if (std::is_reference<T>::value)
                return Formatter<typename std::remove_reference<T>::type>::format(val);
            if (std::is_volatile<T>::value)
                return Formatter<typename std::remove_volatile<T>::type>::format(val);

            return {"?"};
        }
    };

    template<>
    struct Formatter<bool> {
        static std::string format(const bool &val) {
            return val ? "true" : "false";
        }
    };

    template<>
    struct Formatter<int> {
        static std::string format(const int &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<unsigned int> {
        static std::string format(const unsigned int &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<long> {
        static std::string format(const long &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<unsigned long> {
        static std::string format(const unsigned long &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<long long> {
        static std::string format(const long long &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<unsigned long long> {
        static std::string format(const unsigned long long &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<double> {
        static std::string format(const double &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<long double> {
        static std::string format(const long double &val) {
            return fakeit::to_string(val);
        }
    };

    template<>
    struct Formatter<float> {
        static std::string format(const float &val) {
            return fakeit::to_string(val);
        }
    };

}
