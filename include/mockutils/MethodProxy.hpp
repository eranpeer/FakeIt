#pragma once

#include "mockutils/union_cast.hpp"

namespace fakeit {

    // A bunch of functions used by the method id mechanism. We use a pointer to them to represent the ID of a unique
    // mockable method.
    // Those methods are never called, we only use their address, this is why they are excluded from code coverage analysis.
    /* LCOV_EXCL_START */
    template<typename MethodPtrType, MethodPtrType MethodPtr>
    void funcIdMethod() {}

    inline void funcIdDestructor() {}

    inline void funcIdNotStubbed() {}
    /* LCOV_EXCL_STOP */

    using MethodIdType = void(*)();

    struct MethodProxy {

        MethodProxy(MethodIdType id, unsigned int offset, void *vMethod) :
                _id(id),
                _offset(offset),
                _vMethod(vMethod) {
        }

        unsigned int getOffset() const {
            return _offset;
        }

        MethodIdType getId() const {
            return _id;
        }

        void *getProxy() const {
            return union_cast<void *>(_vMethod);
        }

    private:
        MethodIdType _id;
        unsigned int _offset;
        void *_vMethod;
    };
}
