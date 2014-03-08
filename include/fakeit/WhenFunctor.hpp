/*
 * WhenFunctor.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: eran
 */

#ifndef WHENFUNCTOR_HPP_
#define WHENFUNCTOR_HPP_

#include "fakeit/StubbingImpl.h"
#include "fakeit/Stubbing.h"

namespace fakeit {

class WhenFunctor {
public:

	struct StubbingProgress {

		friend class WhenFunctor;

		virtual ~StubbingProgress() THROWS {

			if (std::uncaught_exception()) {
				return;
			}

			if (!_isActive) {
				return;
			}

			_xaction.apply();
		}

		StubbingProgress(StubbingProgress& other) :
				_isActive(other._isActive), _xaction(other._xaction) {
			other._isActive = false; // all other ctors should init _isActive to true;
		}

		StubbingProgress(Xaction& xaction) :
				_isActive(true), _xaction(xaction) {
		}

	protected:

	private:

		bool _isActive;
		Xaction& _xaction;
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

		virtual FirstFunctionStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<BehaviorMock<R, arglist...> > ptr) override {
			root.AppendAction(ptr);
			return *this;
		}

	private:

		FunctionStubbingRoot<C, R, arglist...>& root;
//		FunctionProgress & operator=(const FunctionProgress & other) = delete;
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

		virtual FirstProcedureStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<BehaviorMock<R, arglist...> > ptr) override {
			root.AppendAction(ptr);
			return *this;
		}

	private:
		ProcedureStubbingRoot<C, R, arglist...>& root;
//		ProcedureProgress & operator=(const ProcedureProgress & other) = delete;
	};

	WhenFunctor() {
	}

	template<typename C, typename R, typename ... arglist>
	ProcedureProgress<C, R, arglist...> operator()(const ProcedureStubbingRoot<C, R, arglist...>& stubbingProgress) {
		ProcedureStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<ProcedureStubbingRoot<C, R, arglist...>&>(stubbingProgress);
		//return dynamic_cast<FirstProcedureStubbingProgress<R, arglist...>&>(rootWithoutConst);
		ProcedureProgress<C, R, arglist...> a(rootWithoutConst);
		return a;
	}

	template<typename C, typename R, typename ... arglist>
	FunctionProgress<C, R, arglist...> operator()(const FunctionStubbingRoot<C, R, arglist...>& stubbingProgress) {
		FunctionStubbingRoot<C, R, arglist...>& rootWithoutConst = const_cast<FunctionStubbingRoot<C, R, arglist...>&>(stubbingProgress);
		//return dynamic_cast<FirstFunctionStubbingProgress<R, arglist...>&>(rootWithoutConst);
		FunctionProgress<C, R, arglist...> a(rootWithoutConst);
		return a;
	}

}static When;

}

#endif /* WHENFUNCTOR_HPP_ */
