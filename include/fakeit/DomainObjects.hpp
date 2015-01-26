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

	static unsigned int nextMethodOrdinal(){
		static std::atomic_uint ordinal{ 0 };
		return ++ordinal;
	}

struct FakeitContext;

template<typename C>
struct MockObject {
	virtual ~MockObject() = default;
	virtual C & get() = 0;
	virtual FakeitContext & getFakeIt() = 0;
};

struct MethodInfo {

	MethodInfo(unsigned int id, std::string name):
		_id(id),_name(name)
	{}

	unsigned int id() const {
		return _id;
	}

	std::string name() const {
		return _name;
	}

	void setName(const std::string& name) {
		_name = name;
	}

private:
	unsigned int _id; 
	std::string _name;
};

struct UnknownMethod {

	static MethodInfo & instance() {
		static MethodInfo instance (nextMethodOrdinal(), "unknown");
		return instance;
	}

};

}

#endif // DomainObjects_h__
