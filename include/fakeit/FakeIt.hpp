/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeIt_h__
#define FakeIt_h__

#include "fakeit/DefaultEventHandler.hpp"
#include "fakeit/DefaultErrorFormatter.hpp"

namespace fakeit {


struct FakeIt {

	static void handle(const UnexpectedMethodCallException& e){
		getEventHandler().handle(e);
	}

	static void handle(const SequenceVerificationException& e){
		getEventHandler().handle(e);
	}

	static void handle(const NoMoreInvocationsVerificationException& e){
		getEventHandler().handle(e);
	}

	static EventHandler& getEventHandler(){
		static DefaultEventHandler eventHandler;
		return eventHandler;
	}

};

}


#endif //
