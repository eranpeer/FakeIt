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
		struct FunctionStubbingProgress :
			public virtual FirstFunctionStubbingProgress<R, arglist...>,
			public virtual NextFunctionStubbingProgress<R, arglist...>
		{

			FunctionStubbingProgress() = default;
			~FunctionStubbingProgress() override = default;

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
			virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() = 0;
		private:
			FunctionStubbingProgress & operator= (const FunctionStubbingProgress & other) = delete;
		};



		template <typename R, typename... arglist>
		class FunctionStubbingRoot : public virtual FirstFunctionStubbingProgress<R, arglist...>,
			private virtual FunctionStubbingProgress<R, arglist...>{
		private:
			MethodMock<R, arglist...>& methodMock;
			FunctionStubbingRoot & operator= (const FunctionStubbingRoot & other) = delete;
			MethodInvocationMockBase<R, arglist...>* invocationMock;
		protected:
			virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override  {
				return *invocationMock;
			}

		public:
			FunctionStubbingRoot(MethodMock<R, arglist...>& methodMock) : 
				FunctionStubbingProgress(), 
				FirstFunctionStubbingProgress(), 
				methodMock(methodMock),
				invocationMock(nullptr)
			{
				auto initialMethodBehavior = [](const arglist&... args)->R&{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

			virtual ~FunctionStubbingRoot() override {
				if (invocationMock)
					methodMock.stubMethodInvocation(invocationMock);
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
				return FunctionStubbingProgress::Do(method);
			}
		};

		template <typename R, typename... arglist>
		struct ProcedureStubbingProgress :
			public virtual FirstProcedureStubbingProgress<R, arglist...>,
			public virtual NextProcedureStubbingProgress<R, arglist...>
		{

			ProcedureStubbingProgress() = default;
			~ProcedureStubbingProgress() override = default;

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
			virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() = 0;
		private:
			ProcedureStubbingProgress & operator= (const ProcedureStubbingProgress & other) = delete;
		};

		template <typename R, typename... arglist>
		class ProcedureStubbingRoot : public virtual FirstProcedureStubbingProgress<R, arglist...>,
			private virtual ProcedureStubbingProgress<R, arglist...>{
		private:
			MethodMock<R, arglist...>& methodMock;
			ProcedureStubbingRoot & operator= (const ProcedureStubbingRoot & other) = delete;
			MethodInvocationMockBase<R, arglist...>* invocationMock;
		protected:
			virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override  {
				return *invocationMock;
			}

		public:
			ProcedureStubbingRoot(MethodMock<R, arglist...>& methodMock) : 
				ProcedureStubbingProgress(), 
				FirstProcedureStubbingProgress(), 
				methodMock(methodMock), 
				invocationMock(nullptr)
			{
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

			virtual ~ProcedureStubbingRoot() override {
				if (invocationMock)
					methodMock.stubMethodInvocation(invocationMock);
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
				return ProcedureStubbingProgress::Do(method);
			}
		};

	}
}
#endif // ClousesImpl_h__