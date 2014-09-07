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

	template<typename R, typename ... arglist>
	struct FunctionProgress: public StubbingProgress, public FunctionStubbingProgress<R, arglist...> //
	{
		friend class WhenFunctor;

		virtual ~FunctionProgress() = default;

		FunctionProgress(FunctionProgress& other) :
				StubbingProgress(other), root(other.root) {
		}

		FunctionProgress(StubbingContext<R, arglist...>& xaction) :
				StubbingProgress(xaction), root(xaction) {
		}
	protected:

		virtual FunctionStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...>* action) override {
			root.appendAction(action);
			return *this;
		}

	private:
		StubbingContext<R, arglist...> & root;
	};

	template<typename R, typename ... arglist>
	struct ProcedureProgress: public StubbingProgress, public ProcedureStubbingProgress<R, arglist...> {

		friend class WhenFunctor;

		virtual ~ProcedureProgress() override = default;

		ProcedureProgress(ProcedureProgress& other) :
				StubbingProgress(other), _root(other._root) {
		}

		ProcedureProgress(StubbingContext<R, arglist...>& xaction) :
				StubbingProgress(xaction), _root(xaction) {
		}

	protected:

		virtual ProcedureStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...> * action) override {
			_root.appendAction(action);
			return *this;
		}

	private:
		StubbingContext<R, arglist...>& _root;
	};

	WhenFunctor() {
	}

	template<typename R, typename ... arglist>
	typename std::enable_if<std::is_void<R>::value, ProcedureProgress<R, arglist...>>::type
	operator()(const StubbingContext<R, arglist...>& stubbingContext) {
		StubbingContext<R, arglist...>& rootWithoutConst = const_cast<StubbingContext<R, arglist...>&>(stubbingContext);
		ProcedureProgress<R, arglist...> a(rootWithoutConst);
		return a;
	}

	template<typename R, typename ... arglist>
	typename std::enable_if<!std::is_void<R>::value, FunctionProgress<R, arglist...>>::type
	operator()(const StubbingContext<R, arglist...>& stubbingContext) {
		StubbingContext<R, arglist...>& rootWithoutConst = const_cast<StubbingContext<R, arglist...>&>(stubbingContext);
		FunctionProgress<R, arglist...> a(rootWithoutConst);
		return a;
	}

};

}

#endif /* WHENFUNCTOR_HPP_ */
