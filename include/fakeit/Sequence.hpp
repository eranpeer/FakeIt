/*
 * Sequence.hpp
 *
 *  Created on: Feb 15, 2014
 *      Author: eran
 */

#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include <unordered_set>
#include <vector>
#include <stdexcept>

#include "mockutils/Macros.hpp"
#include "fakeit/ActualInvocation.hpp"

namespace fakeit {

class Sequence {
private:

protected:

	Sequence() {
	}

	virtual ~Sequence() THROWS {
	}

public:

	/**
	 * Fetch the matchers that make-up this sequence.
	 */
	virtual void getExpectedSequence(std::vector<AnyInvocation::Matcher*>& into) const = 0;

	/**
	 * Collect all mock objects that are involved in this sequence.
	 */
	virtual void getInvolvedMocks(std::set<ActualInvocationsSource*>& into) const = 0;

	virtual unsigned int size() const = 0;

	friend class VerifyFunctor;
};

class ConcatenatedSequence: public virtual Sequence {
private:
	const Sequence &s1;
	const Sequence &s2;

protected:
	ConcatenatedSequence(const Sequence &s1, const Sequence &s2) :
			s1(s1), s2(s2) {
	}

public:

	virtual ~ConcatenatedSequence() {
	}

	unsigned int size() const override {
		return s1.size() + s2.size();
	}

	void getExpectedSequence(std::vector<AnyInvocation::Matcher*>& into) const override {
		s1.getExpectedSequence(into);
		s2.getExpectedSequence(into);
	}

	virtual void getInvolvedMocks(std::set<ActualInvocationsSource*>& into) const override {
		s1.getInvolvedMocks(into);
		s2.getInvolvedMocks(into);
	}

	friend inline ConcatenatedSequence operator+(const Sequence &s1, const Sequence &s2);
};

class RepeatedSequence: public virtual Sequence {
private:
	const Sequence &s1;
	const int times;

protected:
	RepeatedSequence(const Sequence &s1, const int times) :
			s1(s1), times(times) {
	}

public:

	~RepeatedSequence() {
	}

	unsigned int size() const override {
		return s1.size() * times;
	}

	friend inline RepeatedSequence operator*(const Sequence &s, int times);
	friend inline RepeatedSequence operator*(int times, const Sequence &s);

	void getInvolvedMocks(std::set<ActualInvocationsSource*>& into) const override {
		s1.getInvolvedMocks(into);
	}

	void getExpectedSequence(std::vector<AnyInvocation::Matcher*>& into) const override {
		for (int i = 0; i < times; i++)
			s1.getExpectedSequence(into);
	}
};

inline ConcatenatedSequence operator+(const Sequence &s1, const Sequence &s2) {
	return ConcatenatedSequence(s1, s2);
}

inline RepeatedSequence operator*(const Sequence &s, int times) {
	if (times <= 0)
		throw std::invalid_argument("times");
	return RepeatedSequence(s, times);
}

inline RepeatedSequence operator*(int times, const Sequence &s) {
	if (times <= 0)
		throw std::invalid_argument("times");
	return RepeatedSequence(s, times);
}

}

#endif /* SEQUENCE_HPP_ */
