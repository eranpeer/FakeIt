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
			public virtual FirstFunctionStubbingProgress<R, arglist...>,
			public virtual NextFunctionStubbingProgress<R, arglist...>
		{

			FunctionWhenClouse() = default;
			~FunctionWhenClouse() override = default;

			NextFunctionStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				InvocationMock().appendDo(method);
				return *this;
			}

			NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}

		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() = 0;
		private:
			FunctionWhenClouse & operator= (const FunctionWhenClouse & other) = delete;
		};



		template <typename R, typename... arglist>
		class FunctionStubbingProgress : public virtual FirstFunctionStubbingProgress<R, arglist...>,
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
			FunctionStubbingProgress(MethodMock<R, arglist...>& methodMock) : FunctionWhenClouse(), FirstFunctionStubbingProgress(), methodMock(methodMock) {
				auto initialMethodBehavior = [](const arglist&... args)->R&{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			FunctionStubbingProgress(const FunctionStubbingProgress& other) = default;

			virtual ~FunctionStubbingProgress() override {
				methodMock.addMethodCall(invocationMock);
			};

			virtual void operator=(std::function<R(arglist...)> method) override {
				// Must override since the implementation in base class is privately inherited
				FirstFunctionStubbingProgress::operator = (method);
			}

			FirstFunctionStubbingProgress<R, arglist...>& Using(const arglist&... args) {
				invocationMock = new ExpectedInvocationMock<R, arglist...>(args...);
				return *this;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				// Must override since the implementation in base class is privately inherited
				return FunctionWhenClouse::Do(method);
			}
		};

		template <typename R, typename... arglist>
		struct ProcedureWhenClouse :
			public virtual FirstProcedureStubbingProgress<R, arglist...>,
			public virtual NextProcedureStubbingProgress<R, arglist...>
		{

			ProcedureWhenClouse() = default;
			~ProcedureWhenClouse() override = default;

			NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				InvocationMock().appendDo(method);
				return *this;
			}

			NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}

		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() = 0;
		private:
			ProcedureWhenClouse & operator= (const ProcedureWhenClouse & other) = delete;
		};

		template <typename R, typename... arglist>
		class ProcedureStubbingProgress : public virtual FirstProcedureStubbingProgress<R, arglist...>,
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
			ProcedureStubbingProgress(MethodMock<R, arglist...>& methodMock) : ProcedureWhenClouse(), FirstProcedureStubbingProgress(), methodMock(methodMock) {
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			ProcedureStubbingProgress(const ProcedureStubbingProgress& other) = default;

			virtual ~ProcedureStubbingProgress() override {
				methodMock.addMethodCall(invocationMock);
			};

			virtual void operator=(std::function<R(arglist...)> method) override {
				// Must override since the implementation in base class is privately inherited
				FirstProcedureStubbingProgress::operator = (method);
			}

			FirstProcedureStubbingProgress<R, arglist...>& Using(const arglist&... args) {
				invocationMock = new ExpectedInvocationMock<R, arglist...>(args...);
				return *this;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
				// Must override since the implementation in base class is privately inherited
				return ProcedureWhenClouse::Do(method);
			}
		};

	}
}
#endif // ClousesImpl_h__