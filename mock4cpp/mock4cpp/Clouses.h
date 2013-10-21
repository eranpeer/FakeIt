#ifndef Clouses_h__
#define Clouses_h__

#include <functional>
#include <type_traits>
#include "../mockutils/traits.h"
#include "../mockutils/DefaultValue.hpp"

namespace mock4cpp {
	namespace clouses {


		struct VerifyClouse {
			virtual void Never() = 0;
			virtual void Once() = 0;
			virtual void Times(const int times) = 0;
		};
		
		template <typename R, typename... arglist>
		struct NextFunctionWhenClouse {

			virtual ~NextFunctionWhenClouse() {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
				ThenReturn(const R& r) {
					return ThenDo(std::function<R(arglist...)>([r](...)->R{
						return r;
					}));
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
				ThenReturn(const R& r) {
					return ThenDo(std::function<R(arglist...)>([&r](...)->R{
						return r;
					}));
				}

			template <typename E>
			NextFunctionWhenClouse<R, arglist...>& ThenThrow(const E& e) {
				return ThenDo(std::function<R(arglist...)>([e](...)->R{throw e; }));
			}

			NextFunctionWhenClouse<R, arglist...>& ThenDo(R(*method)(arglist...)) {
				return ThenDo(std::function<R(arglist...)>(method));
			}

			virtual  NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;

		};


		template <typename R, typename... arglist>
		struct FirstFunctionWhenClouse {
			virtual ~FirstFunctionWhenClouse() = 0 {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
				Return(const R& r) {
					return Do(std::function<R(arglist...)>([r](...)->R{
						return r;
					}));
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
				Return(const R& r) {
					return Do(std::function<R(arglist...)>([&r](...)->R{
						return r;
					}));
				}

			template <typename E>
			NextFunctionWhenClouse<R, arglist...>& Throw(const E& e)  {
				return Do(std::function<R(arglist...)>([e](...)->R{throw e; }));
			}

			void operator=(std::function<R(arglist...)> method){
				Do(method);
			}

			virtual  NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;

		};

		template <typename R, typename... arglist>
		struct NextProcedureWhenClouse {
			virtual ~NextProcedureWhenClouse() = 0 {};

			NextProcedureWhenClouse<R, arglist...>& ThenReturn() {
				return ThenDo(std::function<R(arglist...)>([](...)->R{ return DefaultValue::value<R>(); }));
			}

			template <typename E>
			NextProcedureWhenClouse<R, arglist...>& ThenThrow(const E e) {
				return ThenDo(std::function<R(arglist...)>([e](...)->R{ throw e; }));
			}

			virtual  NextProcedureWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;
		};


		template <typename R, typename... arglist>
		struct FirstProcedureWhenClouse {

			virtual ~FirstProcedureWhenClouse() = 0 {};

			NextProcedureWhenClouse<R, arglist...>& Return() {
				return Do(std::function<R(arglist...)>([](...)->R{ return DefaultValue::value<R>(); }));
			}

			template <typename E>
			NextProcedureWhenClouse<R, arglist...>& Throw(const E e) {
				return Do(std::function<R(arglist...)>([e](...)->R{ throw e; }));
			}

			void operator=(std::function<R(arglist...)> method){
				Do(method);
			}

			virtual  NextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;
		};

		template <typename... arglist>
		struct StubDataMemberClouse {

			virtual ~StubDataMemberClouse() = 0 {};

			virtual void operator()(std::initializer_list<arglist...> list) = 0;
		};
	}
}

#endif // Clouses_h__