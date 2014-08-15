/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeitExceptions_h__
#define FakeitExceptions_h__

#include <functional>
#include <memory>

#include "fakeit/Sequence.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/ErrorFormatter.hpp"

#include "mockutils/Formatter.hpp"
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

	UnexpectedMethodCallException(std::shared_ptr<Invocation> actualInvocation) {
		_actualInvocation = actualInvocation;
	}

	virtual std::string what() const override {
		return getErrorFormatter()->format(*this);
		//return std::string("UnexpectedMethodCallException: could not find any recorded behavior to support this method call");
	}

	const Method& getMethod() const {
		return _actualInvocation->getMethod();
	}

	Invocation& getInvocation() const {
		fakeit::Invocation & invocation = *_actualInvocation;
		return invocation;
	}

private:
	std::shared_ptr<Invocation> _actualInvocation;
};


enum class VerificationType {
	Exact, AtLeast, NoMoreInvocatoins
};

struct VerificationException: public FakeitException {
	virtual ~VerificationException() = default;
	virtual VerificationType verificationType() const = 0;

	void setFileInfo(std::string file, int line, std::string callingMethod) {
		_file = file;
		_callingMethod = callingMethod;
		_line = line;
	}

	const std::string& file() const {return _file;}
	int line() const {return _line;}
	const std::string& 	callingMethod() const { return _callingMethod; }

private:
	std::string _file;
	int _line;
	std::string _callingMethod;
};

struct NoMoreInvocationsVerificationException: public VerificationException {
	NoMoreInvocationsVerificationException( //
			std::vector<Invocation*>& allIvocations, //
			std::vector<Invocation*>& unverifedIvocations) : //
			VerificationException(), _allIvocations(allIvocations), _unverifedIvocations(unverifedIvocations) { //
	}

	virtual VerificationType verificationType() const override {
		return VerificationType::NoMoreInvocatoins;
	}

	const std::vector<Invocation*>& allIvocations() const {
		return _allIvocations;
	}

	const std::vector<Invocation*>& unverifedIvocations() const {
		return _unverifedIvocations;
	}

	virtual std::string what() const override {
//		return FakeIt::getErrorFormatter().format(*this);
		return std::string("VerificationException: expected no more invocations but found ") //
		.append(std::to_string(unverifedIvocations().size()));
	}

private:
	const std::vector<Invocation*> _allIvocations;
	const std::vector<Invocation*> _unverifedIvocations;
};


struct SequenceVerificationException: public VerificationException {
	SequenceVerificationException( //
			std::vector<Sequence*>& expectedPattern, //
			std::vector<Invocation*>& actualSequence, //
			int expectedCount, //
			int actualCount) : //
			VerificationException(), //
			_expectedPattern(expectedPattern), //
			_actualSequence(actualSequence), //
			_expectedCount(expectedCount), //
			_actualCount(actualCount) //
	{ //
	}

	const std::vector<Sequence*>& expectedPattern() const {
		return _expectedPattern;
	}

	const std::vector<Invocation*>& actualSequence() const {
		return _actualSequence;
	}

	const int expectedCount() const {
		return _expectedCount;
	}

	const int actualCount() const {
		return _actualCount;
	}

	virtual std::string what() const override {
//		return FakeIt::getErrorFormatter().format(*this);
		return std::string("VerificationException: expected ") //
		.append(verificationType() == fakeit::VerificationType::Exact ? "exactly " : "at least ") //
		.append(std::to_string(expectedCount())) //
		.append(" invocations but was ") //
		.append(std::to_string(actualCount()));
	}

private:
	const std::vector<Sequence*> _expectedPattern;
	const std::vector<Invocation*> _actualSequence;
	const int _expectedCount;
	const int _actualCount;
};

}
#endif // FakeitExceptions_h__
