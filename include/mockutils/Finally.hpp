/*
 * Finally.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Aug 30, 2014
 */
#pragma once

#include <functional>

namespace fakeit {

    class Finally {
    private:
        std::function<void()> _finallyClause;

        Finally(const Finally &) = delete;

        Finally &operator=(const Finally &) = delete;

    public:
        explicit Finally(std::function<void()> f) :
                _finallyClause(f) {
        }

        Finally(Finally&& other) {
             _finallyClause.swap(other._finallyClause);
        }

        ~Finally() {
            _finallyClause();
        }
    };
}
