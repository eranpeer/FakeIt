/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef ExtractMemberType_h__
#define ExtractMemberType_h__

#include <typeinfo>

namespace fakeit {

template<typename T> struct ExtractMemberTypeHelper;
template<typename R, typename T>
struct ExtractMemberTypeHelper<R (T::*)> {
	typedef R Type;
	typedef T ParentType;
};

template<typename T>
struct ExtractMemberType: public ExtractMemberTypeHelper<T> {
};

}
#endif // ExtractMemberType_h__

