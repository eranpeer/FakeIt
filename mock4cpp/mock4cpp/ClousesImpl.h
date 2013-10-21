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
			VerifyClouseImpl(int times) :times(times){}
			virtual void Never() override {
				if (times != 0)
					throw (std::string("expected Never but was ") + std::to_string(times));
			}
			virtual void Once() override {
				if (times != 1)
					throw (std::string("expected Once but was ") + std::to_string(times));
			}
			virtual void Times(const int times) override {
				if (times != 2)
					throw (std::string("expected Twice but was ") + std::to_string(times));
			}
		private:
			int times;
			VerifyClouseImpl & operator= (const VerifyClouseImpl & other) = delete;
		};

		template <typename R, typename... arglist>
		struct FunctionWhenClouse :
			public FirstFunctionWhenClouse<R, arglist...>,
			public NextFunctionWhenClouse<R, arglist...>
		{

			FunctionWhenClouse(MethodInvocationMock<R, arglist...> * invocationMock) :
				invocationMock(invocationMock)
			{
				ThenDo([](...)->R{ return DefaultValue::value<R>();	});
			}

			~FunctionWhenClouse(){}

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
			FunctionWhenClouse & operator= (const FunctionWhenClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		struct StubFunctionClouse : public FirstFunctionWhenClouse<R, arglist...> {
			StubFunctionClouse(MethodMock<R, arglist...>& methodMock) : methodMock(methodMock) {
			}

			virtual void operator=(std::function<R(arglist...)> method) {
				Do(method);
			}

			FirstFunctionWhenClouse<R, arglist...>& When(const arglist&... args) {
				MethodInvocationMock<R, arglist...> * invocationMock = methodMock.stubMethodCall(args...);
				FunctionWhenClouse<R, arglist...> * whenClouse = new FunctionWhenClouse<R, arglist...>
					(invocationMock);
				return *whenClouse;
			}

			VerifyClouse& Verify(const arglist&... args)  {
				auto verifyClouse = new VerifyClouseImpl(methodMock.getActualInvocations(args...).size());
				return *verifyClouse;
			}

			NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method)  {
				FunctionWhenClouse<R, arglist...> * whenClouse = new FunctionWhenClouse<R, arglist...>(methodMock.last());
				whenClouse->Do(method);
				return *whenClouse;
			}

		private:
			MethodMock<R, arglist...>& methodMock;
			StubFunctionClouse & operator= (const StubFunctionClouse & other) = delete;
		};


		template <typename R, typename... arglist>
		struct ProcedureWhenClouse :
			public FirstProcedureWhenClouse<R, arglist...>,
			public NextProcedureWhenClouse<R, arglist...>{

				ProcedureWhenClouse(MethodInvocationMock<R, arglist...>* invocationMock) :
					invocationMock(invocationMock)
				{
					ThenDo([](...)->R{ return DefaultValue::value<R>();	});
				}

				virtual ~ProcedureWhenClouse() {}

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
			ProcedureWhenClouse & operator= (const ProcedureWhenClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		struct StubProcedureClouse : public FirstProcedureWhenClouse<R, arglist...> {
			StubProcedureClouse(MethodMock<R, arglist...>* methodMock) : methodMock(methodMock),isLast(true) {
			}

			StubProcedureClouse(StubProcedureClouse& other) :methodMock(other.methodMock), isLast(other.isLast){
				other.isLast = false;
			}

			~StubProcedureClouse() {
				if (isLast){
					//int a = isLast;
				}
			}

			virtual void operator=(std::function<R(arglist...)> method)  {
				Do(method);
			}

			FirstProcedureWhenClouse<R, arglist...>& When(const arglist&... args)  {
				MethodInvocationMock<R, arglist...> * invocationMock = methodMock->stubMethodCall(args...);
				ProcedureWhenClouse<R, arglist...> * whenClouse = new ProcedureWhenClouse<R, arglist...>(invocationMock);
				return *whenClouse;
			};

			VerifyClouse & Verify(const arglist&... args)  {
				auto verifyClouse = new VerifyClouseImpl(methodMock->getActualInvocations(args...).size());
				return *verifyClouse;
			}

			NextProcedureWhenClouse<R, arglist...>& Do(std::function<void(arglist...)> method) override {
				ProcedureWhenClouse<R, arglist...> * whenClouse = new ProcedureWhenClouse<R, arglist...>(methodMock->last());
				whenClouse->Do(method);
				return *whenClouse;
			}

		private:
			MethodMock<R, arglist...>* methodMock;
			bool isLast;
			StubProcedureClouse & operator= (const StubProcedureClouse & other) = delete;
		};

		template <typename... arglist>
		struct StubDataMemberClouseImpl : public StubDataMemberClouse<arglist...> {
			virtual void operator()(std::initializer_list<arglist...> list) override {

			};
		};
	}
}
#endif // ClousesImpl_h__