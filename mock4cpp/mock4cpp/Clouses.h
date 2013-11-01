#ifndef Clouses_h__
#define Clouses_h__

#include <functional>
#include <type_traits>
#include "../mockutils/traits.h"
#include "../mockutils/DefaultValue.hpp"

namespace mock4cpp {

	namespace clouses {
		
		template <typename R, typename... arglist>
		struct AbstractNextFunctionWhenClouse {

			virtual ~AbstractNextFunctionWhenClouse() {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<is_copy_initializable<NO_REF>::value, AbstractNextFunctionWhenClouse<R, arglist...>&>::type
				ThenReturn(const R& r) {
					return ThenDo(std::function<R(arglist...)>([r](...)->R{
						return r;
					}));
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!is_copy_initializable<NO_REF>::value, AbstractNextFunctionWhenClouse<R, arglist...>&>::type
				ThenReturn(const R& r) {
					return ThenDo(std::function<R(arglist...)>([&r](...)->R{
						return r;
					}));
				}

			template <typename E>
			AbstractNextFunctionWhenClouse<R, arglist...>& ThenThrow(const E& e) {
				return ThenDo(std::function<R(arglist...)>([e](...)->R{throw e; }));
			}

			AbstractNextFunctionWhenClouse<R, arglist...>& ThenDo(R(*method)(arglist...)) {
				return ThenDo(std::function<R(arglist...)>(method));
			}

			virtual  AbstractNextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;

		private:
			AbstractNextFunctionWhenClouse & operator= (const AbstractNextFunctionWhenClouse & other) = delete;
		};


		template <typename R, typename... arglist>
		struct AbstractFirstFunctionWhenClouse {

			virtual ~AbstractFirstFunctionWhenClouse() {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<std::is_trivially_copy_constructible<NO_REF>::value, AbstractNextFunctionWhenClouse<R, arglist...>&>::type
				Return(const R& r) {
					return Do(std::function<R(arglist...)>([r](...)->R{
						return r;
					}));
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!std::is_trivially_copy_constructible<NO_REF>::value, AbstractNextFunctionWhenClouse<R, arglist...>&>::type
				Return(const R& r) {
					return Do(std::function<R(arglist...)>([&r](...)->R{
						return r;
					}));
				}

			template <typename E>
			AbstractNextFunctionWhenClouse<R, arglist...>& Throw(const E& e)  {
				return Do(std::function<R(arglist...)>([e](...)->R{throw e; }));
			}

			virtual void operator=(std::function<R(arglist...)> method) {
				Do(method);
			}

			virtual  AbstractNextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;
		private:
			AbstractFirstFunctionWhenClouse & operator= (const AbstractFirstFunctionWhenClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		struct NextProcedureWhenClouse {
			virtual ~NextProcedureWhenClouse() {};

			NextProcedureWhenClouse<R, arglist...>& ThenReturn() {
				return ThenDo(std::function<R(arglist...)>([](...)->R{ return DefaultValue::value<R>(); }));
			}

			template <typename E>
			NextProcedureWhenClouse<R, arglist...>& ThenThrow(const E e) {
				return ThenDo(std::function<R(arglist...)>([e](...)->R{ throw e; }));
			}

			virtual  NextProcedureWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;
		private:
			NextProcedureWhenClouse & operator= (const NextProcedureWhenClouse & other) = delete;
		};


		template <typename R, typename... arglist>
		struct FirstProcedureWhenClouse {

			virtual ~FirstProcedureWhenClouse() {};

			NextProcedureWhenClouse<R, arglist...>& Return() {
				return Do(std::function<R(arglist...)>([](...)->R{ return DefaultValue::value<R>(); }));
			}

			template <typename E>
			NextProcedureWhenClouse<R, arglist...>& Throw(const E e) {
				return Do(std::function<R(arglist...)>([e](...)->R{ throw e; }));
			}

			virtual void operator=(std::function<R(arglist...)> method){
				Do(method);
			}

			virtual  NextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;
		private:
			FirstProcedureWhenClouse & operator= (const FirstProcedureWhenClouse & other) = delete;
		};
	}
}

#endif // Clouses_h__