/*
 * Xaction.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 20, 2014
 */
#ifndef XACTION_HPP_
#define XACTION_HPP_


namespace fakeit {

struct Xaction {
	virtual void commit() = 0;
};
}



#endif /* XACTION_HPP_ */
