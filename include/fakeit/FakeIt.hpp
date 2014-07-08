/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeIt_h__
#define FakeIt_h__

#include "fakeit/DefaultLogger.hpp"

namespace fakeit {

struct FakeIt {

	static void log(UnexpectedMethodCallException& e){
		getLogger().log(e);
	}

	static void log(SequenceVerificationException& e){
		getLogger().log(e);
	}

	static void log(NoMoreInvocationsVerificationException& e){
		getLogger().log(e);
	}

	static Logger& getLogger(){
		static DefaultLogger logger;
		return logger;
	}
};

}


#endif //
