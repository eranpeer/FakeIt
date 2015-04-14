#pragma once

#include "mockutils/union_cast.hpp"

namespace fakeit {

    struct MethodProxy {

        MethodProxy(unsigned int id, unsigned int offset, void *vMethod) :
                _id(id),
                _offset(offset),
                _vMethod(vMethod) {
        }

        unsigned int getOffset() const {
            return _offset;
        }

        unsigned int getId() const {
            return _id;
        }

        void *getProxy() const {
            return union_cast<void *>(_vMethod);
        }

    private:
        unsigned int _id;
        unsigned int _offset;
        void *_vMethod;
    };
}