#ifndef mockito_clouses_h__
#define mockito_clouses_h__

namespace mock4cpp {

	namespace mockito_clouses {

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
#endif // mockito_clouses_h__


