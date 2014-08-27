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

	virtual void handle(const UnexpectedMethodCallEvent& e) = 0;

	virtual void handle(const SequenceVerificationEvent& e) = 0;

	virtual void handle(const NoMoreInvocationsVerificationEvent& e) = 0;

};

}
#endif
