/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeitExceptions_h__
#define FakeitExceptions_h__

#include "fakeit/FakeitEvents.hpp"

namespace fakeit {

struct FakeitException {

	virtual ~FakeitException() = default;

	virtual std::string what() const = 0;

	friend std::ostream & operator<<(std::ostream &os, const FakeitException& val) {
		os << val.what();
		return os;
	}
};


struct UnexpectedMethodCallException: public FakeitException {

	UnexpectedMethodCallException(std::string format) :
			_format(format) {
	}

	virtual std::string what() const override {
		return _format;
	}

private:
	std::string _format;
};

struct VerificationException: public FakeitException {
	virtual ~VerificationException() = default;

	void setFileInfo(std::string file, int line, std::string callingMethod) {
		_file = file;
		_callingMethod = callingMethod;
		_line = line;
	}

	const std::string& file() const {
		return _file;
	}
	int line() const {
		return _line;
	}
	const std::string& callingMethod() const {
		return _callingMethod;
	}

private:
	std::string _file;
	int _line;
	std::string _callingMethod;
};

struct NoMoreInvocationsVerificationException: public VerificationException {

	NoMoreInvocationsVerificationException(std::string format) : //
			_format(format) { //
	}

	virtual std::string what() const override {
		return _format;
	}
private:
	std::string _format;
};

struct SequenceVerificationException: public VerificationException {
	SequenceVerificationException(const std::string& format) : //
			_format(format) //
	{
	}

	virtual std::string what() const override {
		return _format;
	}

private:
	std::string _format;
};

}
#endif // FakeitExceptions_h__
