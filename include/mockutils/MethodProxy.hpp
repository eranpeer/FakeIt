#pragma once

#include "mockutils/union_cast.hpp"

namespace fakeit {

    struct MethodProxy {

        MethodProxy(size_t id, unsigned int offset, void *vMethod) :
                _id(id),
                _offset(offset),
                _vMethod(vMethod) {
        }

        unsigned int getOffset() const {
            return _offset;
        }

        size_t getId() const {
            return _id;
        }

        void *getProxy() const {
            return union_cast<void *>(_vMethod);
        }

    private:
        size_t _id;
        unsigned int _offset;
        void *_vMethod;
    };
}