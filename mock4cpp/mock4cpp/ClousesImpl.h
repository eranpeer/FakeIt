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
			public virtual FirstFunctionWhenClouse<R, arglist...>,
			public virtual NextFunctionWhenClouse<R, arglist...>
		{

			FunctionWhenClouse() = default;
			~FunctionWhenClouse() override = default;

			NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				InvocationMock().appendDo(method);
				return *this;
			}

			NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}

			void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() = 0;
		private:
			FunctionWhenClouse & operator= (const FunctionWhenClouse & other) = delete;
		};



		template <typename R, typename... arglist>
		class FunctionStubbingProgress : public virtual FirstFunctionWhenClouse<R, arglist...>,
			private virtual FunctionWhenClouse<R, arglist...>{
		private:
			MethodMock<R, arglist...>& methodMock;
			FunctionStubbingProgress & operator= (const FunctionStubbingProgress & other) = delete;
			MethodInvocationMock<R, arglist...>* invocationMock;
		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() override  {
				return *invocationMock;
			}

		public:
			FunctionStubbingProgress(MethodMock<R, arglist...>& methodMock) : FunctionWhenClouse(), FirstFunctionWhenClouse(), methodMock(methodMock) {
				auto initialMethodBehavior = [](const arglist&... args)->R&{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			FunctionStubbingProgress(const FunctionStubbingProgress& other) = default;

			virtual ~FunctionStubbingProgress() override {
				methodMock.addMethodCall(invocationMock);
			};

			virtual void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

			FirstFunctionWhenClouse<R, arglist...>& Using(const arglist&... args) {
				this->invocationMock = &methodMock.stubMethodCall(args...);
				return *this;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}
		};

		template <typename R, typename... arglist>
		struct ProcedureWhenClouse :
			public virtual FirstProcedureWhenClouse<R, arglist...>,
			public virtual NextProcedureWhenClouse<R, arglist...>
		{

			ProcedureWhenClouse() = default;
			~ProcedureWhenClouse() override = default;

			NextProcedureWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				InvocationMock().appendDo(method);
				return *this;
			}

			NextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}

			void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() = 0;
		private:
			ProcedureWhenClouse & operator= (const ProcedureWhenClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		class ProcedureStubbingProgress : public virtual FirstProcedureWhenClouse<R, arglist...>,
			private virtual ProcedureWhenClouse<R, arglist...>{
		private:
			MethodMock<R, arglist...>& methodMock;
			ProcedureStubbingProgress & operator= (const ProcedureStubbingProgress & other) = delete;
			MethodInvocationMock<R, arglist...>* invocationMock;
		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() override  {
				return *invocationMock;
			}

		public:
			ProcedureStubbingProgress(MethodMock<R, arglist...>& methodMock) : ProcedureWhenClouse(), FirstProcedureWhenClouse(), methodMock(methodMock) {
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			ProcedureStubbingProgress(const ProcedureStubbingProgress& other) = default;

			virtual ~ProcedureStubbingProgress() override {
				methodMock.addMethodCall(invocationMock);
			};

			virtual void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

			FirstProcedureWhenClouse<R, arglist...>& Using(const arglist&... args) {
				invocationMock = &methodMock.stubMethodCall(args...);
				return *this;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			NextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}
		};

	}
}
#endif // ClousesImpl_h__