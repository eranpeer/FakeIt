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

#include "mockutils/smart_ptr.hpp"
#include "mockutils/Destructable.hpp"

namespace fakeit {

class WhenFunctor {

	struct StubbingProgress {

		friend class WhenFunctor;

		virtual ~StubbingProgress() THROWS {

			if (std::uncaught_exception()) {
				return;
			}

			_xaction.commit();
		}

		StubbingProgress(StubbingProgress& other) :
				_xaction(other._xaction) {
		}

	private:

		StubbingProgress(Xaction& xaction)
				: _xaction(xaction) {
		}

		Xaction& _xaction;
	};

public:

	template<typename R, typename ... arglist>
	struct FunctionProgress: public FunctionStubbingProgress<R, arglist...> //
	{
		friend class WhenFunctor;

		virtual ~FunctionProgress() = default;

		FunctionProgress(FunctionProgress& other) :
				_progress(other._progress), _root(other._root) {
		}

		FunctionProgress(StubbingContext<R, arglist...>& xaction) :
				_progress(new StubbingProgress(xaction)), _root(xaction) {
		}
	protected:

		virtual FunctionStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...>* action) override {
			_root.appendAction(action);
			return *this;
		}

	private:
		smart_ptr<StubbingProgress> _progress;
		StubbingContext<R, arglist...> & _root;
	};

	template<typename R, typename ... arglist>
	struct ProcedureProgress: public ProcedureStubbingProgress<R, arglist...> {

		friend class WhenFunctor;

		virtual ~ProcedureProgress() override = default;

		ProcedureProgress(ProcedureProgress& other) :
			_progress(other._progress), _root(other._root) {
		}

		ProcedureProgress(StubbingContext<R, arglist...>& xaction) :
			_progress(new StubbingProgress(xaction)), _root(xaction) {
		}

	protected:

		virtual ProcedureStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...> * action) override {
			_root.appendAction(action);
			return *this;
		}

	private:
		smart_ptr<StubbingProgress> _progress;
		StubbingContext<R, arglist...>& _root;
	};

	WhenFunctor() {
	}

	template<typename R, typename ... arglist>
	typename std::enable_if<std::is_void<R>::value, ProcedureProgress<R, arglist...>>::type
	operator()(const StubbingContext<R, arglist...>& stubbingContext) {
		StubbingContext<R, arglist...>& rootWithoutConst = const_cast<StubbingContext<R, arglist...>&>(stubbingContext);
		ProcedureProgress<R, arglist...> progress(rootWithoutConst);
		return progress;
	}

	template<typename R, typename ... arglist>
	typename std::enable_if<!std::is_void<R>::value, FunctionProgress<R, arglist...>>::type
	operator()(const StubbingContext<R, arglist...>& stubbingContext) {
		StubbingContext<R, arglist...>& rootWithoutConst = const_cast<StubbingContext<R, arglist...>&>(stubbingContext);
		FunctionProgress<R, arglist...> progress(rootWithoutConst);
		return progress;
	}

};

}

#endif /* WHENFUNCTOR_HPP_ */
