/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef WHENFUNCTOR_HPP_
#define WHENFUNCTOR_HPP_

#include "fakeit/StubbingImpl.hpp"
#include "fakeit/Stubbing.hpp"
#include "fakeit/FakeIt.hpp"

namespace fakeit {

class WhenFunctor {
public:

	struct StubbingProgress {

		friend class WhenFunctor;

		virtual ~StubbingProgress() THROWS {

			if (std::uncaught_exception()) {
				_isActive = false;
			}

			if (!_isActive) {
				return;
			}

			_recordedMethodInvocation.apply();
		}

		StubbingProgress(StubbingProgress& other) :
				_isActive(other._isActive), _recordedMethodInvocation(other._recordedMethodInvocation) {
			other._isActive = false; // all other ctors should init _isActive to true;
		}

		StubbingProgress(RecordedMethodInvocation& xaction) :
				_isActive(true), _recordedMethodInvocation(xaction) {
		}

	private:

		bool _isActive;
		RecordedMethodInvocation& _recordedMethodInvocation;
	};

	template<typename C, typename R, typename ... arglist>
	struct FunctionProgress: public StubbingProgress, public FunctionStubbingProgress<R, arglist...> //
	{
		friend class WhenFunctor;

		virtual ~FunctionProgress() = default;

		FunctionProgress(FunctionProgress& other) :
				StubbingProgress(other), root(other.root) {
		}

		FunctionProgress(FunctionStubbingRoot<C, R, arglist...>& xaction) :
				StubbingProgress(xaction), root(xaction) {
		}
	protected:

		virtual FirstFunctionStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<Behavior<R, arglist...> > ptr) override {
			root.AppendAction(ptr);
			return *this;
		}

	private:
		FunctionStubbingRoot<C, R, arglist...>& root;
	};

	template<typename C, typename R, typename ... arglist>
	struct ProcedureProgress: public StubbingProgress, public ProcedureStubbingProgress<R, arglist...> {

		friend class WhenFunctor;

		virtual ~ProcedureProgress() override = default;

		ProcedureProgress(ProcedureProgress& other) :
				StubbingProgress(other), root(other.root) {
		}

		ProcedureProgress(ProcedureStubbingRoot<C, R, arglist...>& xaction) :
				StubbingProgress(xaction), root(xaction) {
		}

	protected:

		virtual FirstProcedureStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<Behavior<R, arglist...> > ptr) override {
			root.AppendAction(ptr);
			return *this;
		}

	private:
		ProcedureStubbingRoot<C, R, arglist...>& root;
	};

	WhenFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	ProcedureProgress<C, R, arglist...> operator()(const ProcedureStubbingRoot<C, R, arglist...>& stubbingProgress) {
		ProcedureStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureStubbingRoot<C, R, arglist...>&>(stubbingProgress);
		ProcedureProgress<C, R, arglist...> a(rootWithoutConst);
		return a;
	}

	template<typename C, typename R, typename ... arglist>
	FunctionProgress<C, R, arglist...> operator()(const FunctionStubbingRoot<C, R, arglist...>& stubbingProgress) {
		FunctionStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<FunctionStubbingRoot<C, R, arglist...>&>(stubbingProgress);
		FunctionProgress<C, R, arglist...> a(rootWithoutConst);
		return a;
	}

}static When;

}

#endif /* WHENFUNCTOR_HPP_ */
