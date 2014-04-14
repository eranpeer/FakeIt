/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef MethodProxy_h__
#define MethodProxy_h__

namespace fakeit {

template<typename R, typename ... arglist>
struct MethodProxy {
	virtual ~MethodProxy() = default;
	virtual unsigned int getOffset() = 0;
	virtual void * getProxy() = 0;
};

}
#endif // MethodProxy_h__
