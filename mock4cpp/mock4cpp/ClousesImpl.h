#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>

#include "../mock4cpp/MethodMock.h"
#include "../mock4cpp/Clouses.h"

namespace mock4cpp {
	namespace clouses {

		struct  VerifyClouse
		{
			VerifyClouse(int times) :times(times){}
			virtual void Never() {
				if (times != 0)
					throw (std::string("expected Never but was ") + std::to_string(times));
			}
			virtual void Once() {
				if (times != 1)
					throw (std::string("expected Once but was ") + std::to_string(times));
			}
			virtual void Times(const int times) {
				if (times != 2)
					throw (std::string("expected Twice but was ") + std::to_string(times));
			}
		private:
			int times;
			VerifyClouse & operator= (const VerifyClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		struct FunctionWhenClouse :
			public virtual AbstractFirstFunctionWhenClouse<R, arglist...>,
			public virtual AbstractNextFunctionWhenClouse<R, arglist...>
		{

			FunctionWhenClouse(MethodInvocationMock<R, arglist...> * invocationMock) :
				invocationMock(invocationMock)
			{
				invocationMock->appendDo([](...)->R{ return DefaultValue::value<R>();	});
			}

			~FunctionWhenClouse() override {}

			AbstractNextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				invocationMock->appendDo(method);
				return *this;
			}

			AbstractNextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				invocationMock->clear();
				invocationMock->appendDo(method);
				return *this;
			}

			void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

		private:
			MethodInvocationMock<R, arglist...>* invocationMock;
			FunctionWhenClouse & operator= (const FunctionWhenClouse & other) = delete;
		};


		template <typename R, typename... arglist>
		struct NextFunctionWhenClouse :
			public AbstractNextFunctionWhenClouse<R, arglist...>
		{

			NextFunctionWhenClouse(MethodInvocationMock<R, arglist...> * invocationMock) :AbstractNextFunctionWhenClouse(),
				invocationMock(invocationMock)
			{
			}

			~NextFunctionWhenClouse(){}

			AbstractNextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				invocationMock->appendDo(method);
				return *this;
			}

		private:
			MethodInvocationMock<R, arglist...>* invocationMock;
			NextFunctionWhenClouse & operator= (const NextFunctionWhenClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		struct FirstFunctionWhenClouse :
			public AbstractFirstFunctionWhenClouse<R, arglist...>
		{

			FirstFunctionWhenClouse(MethodInvocationMock<R, arglist...> * invocationMock) :AbstractFirstFunctionWhenClouse(),
				invocationMock(invocationMock)
			{
				invocationMock->appendDo([](...)->R{ return DefaultValue::value<R>();	});
			}

			~FirstFunctionWhenClouse(){}

			AbstractNextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				invocationMock->clear();
				invocationMock->appendDo(method);
				return *(new NextFunctionWhenClouse<R, arglist...>(invocationMock));
			}

			void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

		private:
			MethodInvocationMock<R, arglist...>* invocationMock;
		};



		template <typename R, typename... arglist>
		struct StubFunctionClouse : public AbstractFirstFunctionWhenClouse<R, arglist...> {
			StubFunctionClouse(MethodMock<R, arglist...>& methodMock) : methodMock(methodMock) {
			}

			virtual ~StubFunctionClouse() override {};

			virtual void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

			FirstFunctionWhenClouse<R, arglist...> When(const arglist&... args) {
				MethodInvocationMock<R, arglist...> * invocationMock = methodMock.stubMethodCall(args...);
				return FirstFunctionWhenClouse<R, arglist...> (invocationMock);
// 				FunctionWhenClouse<R, arglist...> * whenClouse = new FunctionWhenClouse<R, arglist...>
// 					(invocationMock);
				//return *whenClouse;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			AbstractNextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method)  {
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

				ProcedureWhenClouse(MethodInvocationMock<R, arglist...>* invocationMock) :FirstProcedureWhenClouse(), NextProcedureWhenClouse(),
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

			StubProcedureClouse(StubProcedureClouse&& other) = delete;
// 				:methodMock(other.methodMock), isLast(other.isLast){
// 				other.isLast = false;
// 			}

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

			VerifyClouse Verify(const arglist&... args)  {
				return new VerifyClouse(methodMock->getActualInvocations(args...).size());
			}

			NextProcedureWhenClouse<R, arglist...>& Do(std::function<void(arglist...)> method) override {
				ProcedureWhenClouse<R, arglist...> * whenClouse = new ProcedureWhenClouse<R, arglist...>(methodMock->last());
				whenClouse->Do(method);
				return *whenClouse;
			}

		private:
			MethodMock<R, arglist...>* methodMock;
			bool isLast;
			StubProcedureClouse<R, arglist...> & operator = (const StubProcedureClouse<R, arglist...> & other) = delete;
			StubProcedureClouse<R, arglist...> & operator = (const StubProcedureClouse<R, arglist...> && other) = delete;
		};
	}
}
#endif // ClousesImpl_h__