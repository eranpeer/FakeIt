#pragma once

#include "mockutils/union_cast.hpp"

namespace fakeit {

    struct MethodProxy {

        MethodProxy(unsigned int offset, void * vMethod) :
            _vMethod(vMethod), _offset(offset) {
        }

        unsigned int getOffset() const {
            return _offset;
        }

        void * getProxy() const {
            return union_cast<void *>(_vMethod);
        }

    private:
        void * _vMethod;
        unsigned int _offset;
    };

}