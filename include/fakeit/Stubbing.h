#ifndef Clouses_h__
#define Clouses_h__
#include "mockutils/Macros.h"

#include <functional>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <stdexcept>

#include "mockutils/traits.h"
#include "mockutils/DefaultValue.hpp"
#include "fakeit/Exceptions.h"
#include "fakeit/InvocationMatcher.h"
#include "fakeit/ActualInvocation.h"

namespace fakeit {

struct MethodVerificationProgress {


	MethodVerificationProgress() {
	}

	virtual ~MethodVerificationProgress() THROWS {
	}

	void Never() {
		Times(0);
	}

	virtual void Once() {
		Times(1);
	}

	virtual void Twice() {
		Times(2);
	}

	virtual void AtLeastOnce() {
		verifyInvocations(-1);
	}

	virtual void Times(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(times);
	}

	virtual void AtLeast(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(-times);
	}

protected:
	virtual void verifyInvocations(const int times) = 0;
private:
	MethodVerificationProgress & operator=(const MethodVerificationProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct NextFunctionStubbingProgress {

	virtual ~NextFunctionStubbingProgress() THROWS {
	}

	NextFunctionStubbingProgress<R, arglist...>&
	ThenReturn(const R& r) {
		return ThenDo([&r](...)->R {return r;});
	}

	NextFunctionStubbingProgress<R, arglist...>&
	ThenReturn() {
		return Do([](...)->R {DefaultValue::value<R>();});
	}

	template<typename E>
	NextFunctionStubbingProgress<R, arglist...>& ThenThrow(const E& e) {
		return ThenDo([e](...)->R {throw e;});
	}

	NextFunctionStubbingProgress<R, arglist...>& ThenDo(R (*method)(arglist...)) {
		return ThenDo(std::function < R(arglist...) > (method));
	}

	virtual auto ThenDo(std::function<R(arglist...)> method) -> NextFunctionStubbingProgress<R, arglist...>& = 0;

private:
	NextFunctionStubbingProgress & operator=(const NextFunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FirstFunctionStubbingProgress {

	virtual ~FirstFunctionStubbingProgress() THROWS {
	}

	NextFunctionStubbingProgress<R, arglist...>&
	Return(const R& r) {
		return Do([&r](...)->R {return r;});
	}

	NextFunctionStubbingProgress<R, arglist...>&
	Return() {
		return Do([](...)->R {DefaultValue::value<R>();});
	}

	template<typename E>
	NextFunctionStubbingProgress<R, arglist...>& Throw(const E& e) {
		return Do([e](...)->R {throw e;});
	}

	virtual void operator=(std::function<R(arglist...)> method) {
		Do(method);
	}

	virtual auto Do(std::function<R(arglist...)> method) -> NextFunctionStubbingProgress<R, arglist...>& = 0;

private:
	FirstFunctionStubbingProgress & operator=(const FirstFunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct NextProcedureStubbingProgress {
	virtual ~NextProcedureStubbingProgress() THROWS {
	}

	NextProcedureStubbingProgress<R, arglist...>& ThenReturn() {
		return ThenDo([](...)->R {return DefaultValue::value<R>();});
	}

	template<typename E>
	NextProcedureStubbingProgress<R, arglist...>& ThenThrow(const E e) {
		return ThenDo([e](...)->R {throw e;});
	}

	virtual NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;
private:
	NextProcedureStubbingProgress & operator=(const NextProcedureStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FirstProcedureStubbingProgress {

	virtual ~FirstProcedureStubbingProgress() THROWS {
	}

	NextProcedureStubbingProgress<R, arglist...>& Return() {
		return Do([](...)->R {return DefaultValue::value<R>();});
	}

	template<typename E>
	NextProcedureStubbingProgress<R, arglist...>& Throw(const E e) {
		return Do([e](...)->R {throw e;});
	}

	virtual void operator=(std::function<R(arglist...)> method) {
		Do(method);
	}

	virtual NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;

private:
	FirstProcedureStubbingProgress & operator=(const FirstProcedureStubbingProgress & other) = delete;
};

class MethodStubbingInternal {
public:

	MethodStubbingInternal() = default;

protected:

	~MethodStubbingInternal() = default;

	virtual void startStubbing() = 0;

};

class Sequence {
private:

protected:

	Sequence() {
	}

	virtual ~Sequence() THROWS {}

public:
	virtual void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const = 0;

	virtual void getExpectedSequence(std::vector<AnyInvocationMatcher*>& into) const = 0;

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

	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
		s1.getActualInvocations(into);
		s2.getActualInvocations(into);
	}

	void getExpectedSequence(std::vector<AnyInvocationMatcher*>& into) const override {
		s1.getExpectedSequence(into);
		s2.getExpectedSequence(into);
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

	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
		s1.getActualInvocations(into);
	}

	void getExpectedSequence(std::vector<AnyInvocationMatcher*>& into) const override {
		for (int i = 0; i < times; i++)
			s1.getExpectedSequence(into);
	}
};

inline ConcatenatedSequence operator+(const Sequence &s1, const Sequence &s2) {
	return ConcatenatedSequence(s1, s2);
}

inline RepeatedSequence operator*(const Sequence &s, int times) {
	return RepeatedSequence(s, times);
}

inline RepeatedSequence operator*(int times, const Sequence &s) {
	return RepeatedSequence(s, times);
}

}

#endif // Clouses_h__
