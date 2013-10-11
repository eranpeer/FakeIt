#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>

#include "../mock4cpp/MethodMock.h"
#include "../mock4cpp/Clouses.h"

namespace mock4cpp {
	namespace clouses {

		struct  VerifyClouseImpl : public VerifyClouse
		{
			virtual void Never() override {};
			virtual void Once() override {};
			virtual void Times(const int times) override {};
		};

		template <typename R, typename... arglist>
		struct FunctionWhenClouseImpl :
			public FirstFunctionWhenClouse<R, arglist...>,
			public NextFunctionWhenClouse<R, arglist...>
		{

			FunctionWhenClouseImpl(MethodInvocationMock<R, arglist...> * invocationMock) :
				invocationMock(invocationMock)
			{
				ThenDo([](...)->R{ return DefaultValue::value<R>();	});
			}

			~FunctionWhenClouseImpl(){}

			NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				invocationMock->appendDo(method);
				return *this;
			}

			NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				invocationMock->clear();
				invocationMock->appendDo(method);
				return *this;
			}

		private:
			MethodInvocationMock<R, arglist...>* invocationMock;
		};

		template <typename R, typename... arglist>
		struct StubFunctionClouseImpl : public StubFunctionClouse<R, arglist...> {
			StubFunctionClouseImpl(MethodMock<R, arglist...>* methodMock) : methodMock(methodMock) {
			}

			FirstFunctionWhenClouse<R, arglist...>& When(const arglist&... args) override {
				MethodInvocationMock<R, arglist...> * invocationMock = methodMock->stubMethodCall(args...);
				FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>
					(invocationMock);
				return *whenClouse;
			}

			VerifyClouse& Verify(const arglist&...) override {
				auto verifyClouse = new VerifyClouseImpl();
				return *verifyClouse;
			}

			NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>(methodMock->last());
				whenClouse->Do(method);
				return *whenClouse;
			}

		private:
			MethodMock<R, arglist...>* methodMock;
		};


		template <typename R, typename... arglist>
		struct ProcedureWhenClouseImpl :
			public FirstProcedureWhenClouse<R, arglist...>,
			public NextProcedureWhenClouse<R, arglist...>{

				ProcedureWhenClouseImpl(MethodInvocationMock<R, arglist...>* invocationMock) :
					invocationMock(invocationMock)
				{
					ThenDo([](...)->R{ return DefaultValue::value<R>();	});
				}

				virtual ~ProcedureWhenClouseImpl() {}

				NextProcedureWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
					invocationMock->appendDo(method);
					return *this;
				}

				NextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
					invocationMock->clear();
					invocationMock->appendDo(method);
					return *this;
				}

		private:
			MethodInvocationMock<R, arglist...>* invocationMock;
		};

		template <typename R, typename... arglist>
		struct StubProcedureClouseImpl : public StubProcedureClouse<R, arglist...> {
			StubProcedureClouseImpl(MethodMock<R, arglist...>* methodMock) : methodMock(methodMock) {
			}

			FirstProcedureWhenClouse<R, arglist...>& When(const arglist&... args) override {
				MethodInvocationMock<R, arglist...> * invocationMock = methodMock->stubMethodCall(args...);
				ProcedureWhenClouseImpl<R, arglist...> * whenClouse = new ProcedureWhenClouseImpl<R, arglist...>(invocationMock);
				return *whenClouse;
			};

			VerifyClouse & Verify(const arglist&...) override {
				auto verifyClouse = new VerifyClouseImpl();
				return *verifyClouse;
			}

			NextProcedureWhenClouse<R, arglist...>& Do(std::function<void(arglist...)> method) override {
				ProcedureWhenClouseImpl<R, arglist...> * whenClouse = new ProcedureWhenClouseImpl<R, arglist...>(methodMock->last());
				whenClouse->Do(method);
				return *whenClouse;
			}

		private:
			MethodMock<R, arglist...>* methodMock;
		};

		template <typename... arglist>
		struct StubDataMemberClouseImpl : public StubDataMemberClouse<arglist...> {
			virtual void operator()(std::initializer_list<arglist...> list) override {

			};
		};
	}
}
#endif // ClousesImpl_h__