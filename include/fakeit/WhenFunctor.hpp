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
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/FakeitContext.hpp"

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

			_recordedMethodInvocation.commit();
		}

		StubbingProgress(StubbingProgress& other) :
				_isActive(other._isActive), _recordedMethodInvocation(other._recordedMethodInvocation) {
			other._isActive = false; // all other ctors should init _isActive to true;
		}

		StubbingProgress(Xaction& xaction) :
				_isActive(true), _recordedMethodInvocation(xaction) {
		}

	private:

		bool _isActive;
		Xaction& _recordedMethodInvocation;
	};

	template<typename C, typename R, typename ... arglist>
	struct FunctionProgress: public StubbingProgress, public FunctionStubbingProgress<R, arglist...> //
	{
		friend class WhenFunctor;

		virtual ~FunctionProgress() = default;

		FunctionProgress(FunctionProgress& other) :
				StubbingProgress(other), root(other.root) {
		}

		FunctionProgress(StubbingContext<C, R, arglist...>& xaction) :
				StubbingProgress(xaction), root(xaction) {
		}
	protected:

		virtual FunctionStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<Action<R, arglist...> > ptr) override {
			root.appendAction(ptr);
			return *this;
		}

	private:
		StubbingContext<C, R, arglist...> & root;
	};

	template<typename C, typename R, typename ... arglist>
	struct ProcedureProgress: public StubbingProgress, public ProcedureStubbingProgress<R, arglist...> {

		friend class WhenFunctor;

		virtual ~ProcedureProgress() override = default;

		ProcedureProgress(ProcedureProgress& other) :
				StubbingProgress(other), _root(other._root) {
		}

		ProcedureProgress(StubbingContext<C, R, arglist...>& xaction) :
				StubbingProgress(xaction), _root(xaction) {
		}

	protected:

		virtual ProcedureStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<Action<R, arglist...> > ptr) override {
			_root.appendAction(ptr);
			return *this;
		}

	private:
		StubbingContext<C, R, arglist...>& _root;
	};

	WhenFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	typename std::enable_if<std::is_void<R>::value, ProcedureProgress<C, R, arglist...>>::type
	operator()(const StubbingContext<C, R, arglist...>& stubbingContext) {
		StubbingContext<C, R, arglist...>& rootWithoutConst = const_cast<StubbingContext<C, R, arglist...>&>(stubbingContext);
		ProcedureProgress<C, R, arglist...> a(rootWithoutConst);
		return a;
	}

	template<typename C, typename R, typename ... arglist>
	typename std::enable_if<!std::is_void<R>::value, FunctionProgress<C, R, arglist...>>::type
	operator()(const StubbingContext<C, R, arglist...>& stubbingContext) {
		StubbingContext<C, R, arglist...>& rootWithoutConst = const_cast<StubbingContext<C, R, arglist...>&>(stubbingContext);
		FunctionProgress<C, R, arglist...> a(rootWithoutConst);
		return a;
	}

}static When;

}

#endif /* WHENFUNCTOR_HPP_ */
