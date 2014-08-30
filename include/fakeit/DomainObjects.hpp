/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DomainObjects_h__
#define DomainObjects_h__

#include <string>

namespace fakeit {

struct FakeitContext;

template<typename C>
struct MockObject {
	virtual ~MockObject() = default;
	virtual C & get() = 0;
	virtual FakeitContext & getFakeIt() = 0;
};

struct Method {
	Method() = default;
	virtual ~Method() = default;
	virtual std::string name() const = 0;
	virtual bool operator == (const Method &other) const = 0;
};

struct UnknownMethod : public Method {

	virtual ~UnknownMethod() = default;

	virtual std::string name() const {
		return {"unknown"};
	}

	static Method& instance() {
		static UnknownMethod instance;
		return instance;
	}

	bool operator == (const Method &other) const override {
		const UnknownMethod* b = dynamic_cast< const UnknownMethod* >( &other );
		return b != NULL;
	}

};

}

#endif // DomainObjects_h__
