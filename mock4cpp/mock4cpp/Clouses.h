#ifndef Clouses_h__
#define Clouses_h__

#include <functional>
#include <type_traits>
#include "../mockutils/traits.h"
#include "../mockutils/DefaultValue.hpp"

namespace mock4cpp {
	namespace stub_clouses {

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
		struct StubFunctionClouse : public FirstFunctionWhenClouse<R, arglist...>{

			virtual ~StubFunctionClouse() = 0 {};

			void operator=(std::function<R(arglist...)> method){
				Do(method);
			}

			virtual  FirstFunctionWhenClouse<R, arglist...>& When(const arglist&...) = 0;
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

		template <typename R, typename... arglist>
		struct StubProcedureClouse : public FirstProcedureWhenClouse<R, arglist...>{

			virtual ~StubProcedureClouse() = 0 {};

			void operator=(std::function<R(arglist...)> method){
				Do(method);
			}

			virtual  FirstProcedureWhenClouse<R, arglist...>& When(const arglist&...) = 0;
		};


		template <typename... arglist>
		struct StubDataMemberClouse {

			virtual ~StubDataMemberClouse() = 0 {};

			virtual void operator()(std::initializer_list<arglist...> list) = 0;
		};
	}


	namespace when_clouses {

		template <typename R>
		struct NextFunctionWhenClouse {

			virtual ~NextFunctionWhenClouse() {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
				ThenReturn(const R& r) {
					return ThenDo(std::function<R()>([r](...)->R{
						return r;
					}));
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
				ThenReturn(const R& r) {
					return ThenDo(std::function<R()>([&r](...)->R{ return r; }));
				}

			template <typename E>
			NextFunctionWhenClouse<R>& ThenThrow(const E& e) {
				return ThenDo(std::function<R()>([e](...)->R{throw e; }));
			}

			NextFunctionWhenClouse<R>& ThenDo(R(*method)()) {
				return ThenDo(std::function<R()>(method));
			}

			virtual  NextFunctionWhenClouse<R>& ThenDo(std::function<R()> method) = 0;

		};


		template <typename R>
		struct FirstFunctionWhenClouse {
			virtual ~FirstFunctionWhenClouse() = 0 {};

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
				Return(const R& r) {
					return Do(std::function<R()>([r](...)->R{ return r; }));
				}

			template<typename NO_REF = std::remove_reference<R>::type>
			typename std::enable_if<!std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
				Return(const R& r) {
					return Do(std::function<R()>([&r](...)->R{ return r; }));
				}

			template <typename E>
			NextFunctionWhenClouse<R>& Throw(const E& e)  {
				return Do(std::function<R()>([e](...)->R{throw e; }));
			}

			virtual  NextFunctionWhenClouse<R>& Do(std::function<R()> method) = 0;

		};

		template <typename R>
		struct NextProcedureWhenClouse {
			virtual ~NextProcedureWhenClouse() = 0 {};

			NextProcedureWhenClouse<R>& ThenReturn() {
				return ThenDo(std::function<R()>([](...)->R{ return DefaultValue::value<R>(); }));
			}

			template <typename E>
			NextProcedureWhenClouse<R>& ThenThrow(const E e) {
				return ThenDo(std::function<R()>([e](...)->R{ throw e; }));
			}

			virtual  NextProcedureWhenClouse<R>& ThenDo(std::function<R()> method) = 0;
		};


		template <typename R>
		struct FirstProcedureWhenClouse {

			virtual ~FirstProcedureWhenClouse() = 0 {};

			NextProcedureWhenClouse<R>& Return() {
				return Do(std::function<R()>([](...)->R{ return DefaultValue::value<R>(); }));
			}

			template <typename E>
			NextProcedureWhenClouse<R>& Throw(const E e) {
				return Do(std::function<R()>([e](...)->R{ throw e; }));
			}

			virtual  NextProcedureWhenClouse<R>& Do(std::function<R()> method) = 0;
		};

	}
}

template <typename R>
typename std::enable_if<!std::is_void<R>::value, mock4cpp::when_clouses::FirstFunctionWhenClouse<R>&>::type
When(const R& r) {
	throw 1;
}

template <typename R>
typename std::enable_if<std::is_void<R>::value, mock4cpp::when_clouses::FirstProcedureWhenClouse<R>&>::type
When(R) {
	throw 1;
}

#endif // Clouses_h__
