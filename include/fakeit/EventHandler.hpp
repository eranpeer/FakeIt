/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef EventHandler_h__
#define EventHandler_h__

namespace fakeit {

struct UnexpectedMethodCallException;
struct SequenceVerificationException;
struct NoMoreInvocationsVerificationException;

struct EventHandler {

	virtual ~EventHandler() = default;

	virtual void handle(const UnexpectedMethodCallException& e) = 0;

	virtual void handle(const SequenceVerificationException& e) = 0;

	virtual void handle(const NoMoreInvocationsVerificationException& e) = 0;

};

}
#endif
