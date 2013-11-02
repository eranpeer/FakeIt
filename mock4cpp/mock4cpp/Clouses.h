#ifndef Clouses_h__
#define Clouses_h__

#include <functional>
#include <type_traits>
#include "../mockutils/traits.h"
#include "../mockutils/DefaultValue.hpp"

namespace mock4cpp {

	namespace clouses {
		
		template <typename R, typename... arglist>
		struct NextFunctionStubbingProgress {

			virtual ~NextFunctionStubbingProgress() {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<is_copy_initializable<NO_REF>::value, NextFunctionStubbingProgress<R, arglist...>&>::type
				ThenReturn(const R& r) {
					return ThenDo([r](...)->R{ return r; });
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!is_copy_initializable<NO_REF>::value, NextFunctionStubbingProgress<R, arglist...>&>::type
				ThenReturn(const R& r) {
					return ThenDo([&r](...)->R{ return r; });
				}

			template <typename E>
			NextFunctionStubbingProgress<R, arglist...>& ThenThrow(const E& e) {
				return ThenDo([e](...)->R{throw e; });
			}

			NextFunctionStubbingProgress<R, arglist...>& ThenDo(R(*method)(arglist...)) {
				return ThenDo(std::function<R(arglist...)>(method));
			}

			virtual  NextFunctionStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;

		private:
			NextFunctionStubbingProgress & operator= (const NextFunctionStubbingProgress & other) = delete;
		};


		template <typename R, typename... arglist>
		struct FirstFunctionStubbingProgress {

			virtual ~FirstFunctionStubbingProgress() {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionStubbingProgress<R, arglist...>&>::type
				Return(const R& r) {
					return Do([r](...)->R{ return r; });
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionStubbingProgress<R, arglist...>&>::type
				Return(const R& r) { 
					return Do([&r](...)->R{ return r; });
				}

			template <typename E>
			NextFunctionStubbingProgress<R, arglist...>& Throw(const E& e)  {
				return Do([e](...)->R{throw e; });
			}

			virtual void operator=(std::function<R(arglist...)> method) {
				Do(method);
			}

			virtual  NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;
		private:
			FirstFunctionStubbingProgress & operator= (const FirstFunctionStubbingProgress & other) = delete;
		};

		template <typename R, typename... arglist>
		struct NextProcedureStubbingProgress {
			virtual ~NextProcedureStubbingProgress() {};

			NextProcedureStubbingProgress<R, arglist...>& ThenReturn() {
				return ThenDo([](...)->R{ return DefaultValue::value<R>(); });
			}

			template <typename E>
			NextProcedureStubbingProgress<R, arglist...>& ThenThrow(const E e) {
				return ThenDo([e](...)->R{ throw e; });
			}

			virtual  NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;
		private:
			NextProcedureStubbingProgress & operator= (const NextProcedureStubbingProgress & other) = delete;
		};


		template <typename R, typename... arglist>
		struct FirstProcedureStubbingProgress {

			virtual ~FirstProcedureStubbingProgress() {};

			NextProcedureStubbingProgress<R, arglist...>& Return() {
				return Do([](...)->R{ return DefaultValue::value<R>(); });
			}

			template <typename E>
			NextProcedureStubbingProgress<R, arglist...>& Throw(const E e) {
				return Do([e](...)->R{ throw e; });
			}

			virtual void operator=(std::function<R(arglist...)> method){
				Do(method);
			}

			virtual  NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;
		private:
			FirstProcedureStubbingProgress & operator= (const FirstProcedureStubbingProgress & other) = delete;
		};
	}
}

#endif // Clouses_h__