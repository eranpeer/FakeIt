#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>

#include "../mock4cpp/MethodMock.h"
#include "../mock4cpp/Stubbing.h"

namespace mock4cpp {

	namespace stubbing {

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

	}

	using namespace mock4cpp;
	using namespace mock4cpp::stubbing;

	template <typename R, typename... arglist>
	class FunctionStubbingRoot : public virtual FirstFunctionStubbingProgress<R, arglist...>,
		private virtual FunctionStubbingProgress<R, arglist...>,
		public virtual verification::FunctionVerificationProgress {
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
		}

		FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

		virtual ~FunctionStubbingRoot() override {
			if (invocationMock)
				methodMock.stubMethodInvocation(invocationMock);
		}

		virtual void operator=(std::function<R(arglist...)> method) override {
			// Must override since the implementation in base class is privately inherited
			FirstFunctionStubbingProgress::operator = (method);
		}

		FirstFunctionStubbingProgress<R, arglist...>& Using(const arglist&... args) {
			invocationMock = new ExpectedInvocationMock<R, arglist...>(args...);
			return *this;
		}

		NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
			// Must override since the implementation in base class is privately inherited
			apply();
			return FunctionStubbingProgress::Do(method);
		}

		virtual int CountInvocations() override {
			apply();
			int times = methodMock.getActualInvocations(*invocationMock).size();
			delete invocationMock;
			invocationMock = nullptr;
			return times;
		}

		MethodInvocationMockBase<R, arglist...>& apply() {
			if (!invocationMock){
				auto initialMethodBehavior = [](const arglist&... args)->R&{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}
			return *invocationMock;
		}
	};

	template <typename R, typename... arglist>
	class ProcedureStubbingRoot : public virtual FirstProcedureStubbingProgress<R, arglist...>,
		private virtual ProcedureStubbingProgress<R, arglist...>,
		public virtual verification::FunctionVerificationProgress {
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
		}

		ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

		virtual ~ProcedureStubbingRoot() override {
			if (invocationMock)
				methodMock.stubMethodInvocation(invocationMock);
		}

		virtual void operator=(std::function<R(arglist...)> method) override {
			// Must override since the implementation in base class is privately inherited
			FirstProcedureStubbingProgress::operator = (method);
		}

		FirstProcedureStubbingProgress<R, arglist...>& Using(const arglist&... args) {
			invocationMock = new ExpectedInvocationMock<R, arglist...>(args...);
			return *this;
		}

		NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
			// Must override since the implementation in base class is privately inherited
			apply();
			return ProcedureStubbingProgress::Do(method);
		}

		virtual int CountInvocations() override {
			apply();
			int times = methodMock.getActualInvocations(*invocationMock).size();
			delete invocationMock;
			invocationMock = nullptr;
			return times;
		}

		void apply(){
			if (!invocationMock){
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}
		}
	};

}
#endif // ClousesImpl_h__