#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>
#include <memory>

#include "../mock4cpp/MethodMock.h"
#include "../mock4cpp/Stubbing.h"
namespace mock4cpp {

	namespace stubbing {

		template <typename R, typename... arglist>
		struct StubbingContext {
			virtual MethodMock<R, arglist...>& getMethodMock() = 0;
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


	static enum ProgressType
	{
		NONE, STUBBING, VERIFYING
	};

	template <typename R, typename... arglist>
	class MethodStubbingBase
		: public virtual verification::MethodVerificationProgress
	{
	protected:
		std::shared_ptr<StubbingContext <R, arglist... >> stubbingContext;
		MethodInvocationMockBase<R, arglist...>* invocationMock;
		ProgressType progressType;
		int expectedInvocationCount;

		MethodStubbingBase(std::shared_ptr < StubbingContext < R, arglist... >> stubbingContext) :
			stubbingContext(stubbingContext),
			invocationMock(nullptr),
			progressType(ProgressType::NONE),
			expectedInvocationCount(-1)
		{
		}

		int CountInvocations(MethodInvocationMockBase<R, arglist...> &invocationMock) {
			int times = stubbingContext->getMethodMock().getActualInvocations(invocationMock).size();
			return times;
		}

		void initInvocationMockIfNeeded(){
			if (!invocationMock){
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}
		}

	public:

		virtual ~MethodStubbingBase() {
			if (progressType == ProgressType::NONE) {
				return;
			}

			initInvocationMockIfNeeded();

			if (progressType == ProgressType::STUBBING){
				stubbingContext->getMethodMock().stubMethodInvocation(invocationMock);
				return;
			}

			if (progressType == ProgressType::VERIFYING){
				auto actualInvocations = CountInvocations(*invocationMock);
				delete invocationMock;
				invocationMock = nullptr;

				if (expectedInvocationCount == -1) {
					if (actualInvocations == 0)
						throw (std::string("no matching invocations"));
					return;
				}
				if (actualInvocations != expectedInvocationCount) {
					throw (std::string("expected ") + std::to_string(expectedInvocationCount) + " but was " + std::to_string(actualInvocations));
				}
			}
		}

		virtual void clearProgress() {
			progressType = ProgressType::NONE;
			if (invocationMock) {
				delete invocationMock;
				invocationMock = nullptr;
			}
		}

		void startStubbing() {
			progressType = ProgressType::STUBBING;
		}

		virtual void startVerification() override {
			progressType = ProgressType::VERIFYING;
		}

		virtual void VerifyInvocations(const int times) override {
			startVerification();
			expectedInvocationCount = times;
		}

	};

	template <typename R, typename... arglist>
	class FunctionStubbingRoot : 
		public virtual MethodStubbingBase<R,arglist...>, 
		public virtual FirstFunctionStubbingProgress<R, arglist...>,
		private virtual FunctionStubbingProgress<R, arglist...>
	{
	private:
		FunctionStubbingRoot & operator= (const FunctionStubbingRoot & other) = delete;
	protected:
		virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override  {
			return *invocationMock;
		}
	public:
		FunctionStubbingRoot(std::shared_ptr<StubbingContext <R, arglist... >> stubbingContext) :
			MethodStubbingBase(stubbingContext),
			FunctionStubbingProgress(),
			FirstFunctionStubbingProgress()
		{
		}

		FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

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
			startStubbing();
			initInvocationMockIfNeeded();
			return FunctionStubbingProgress::Do(method);
		}

		virtual void VerifyInvocations(const int times) override {
			MethodStubbingBase::VerifyInvocations(times);
		}

		virtual void startVerification() override {
			MethodStubbingBase::startVerification();
		}

		virtual void clearProgress() override {
			MethodStubbingBase::clearProgress();
		}
	};

	template <typename R, typename... arglist>
	class ProcedureStubbingRoot : 
		public virtual MethodStubbingBase<R, arglist...>,
		public virtual FirstProcedureStubbingProgress<R, arglist...>,
		private virtual ProcedureStubbingProgress<R, arglist...>
	{
	private:
		ProcedureStubbingRoot & operator= (const ProcedureStubbingRoot & other) = delete;
	protected:
		virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override  {
			return *invocationMock;
		}

	public:
		ProcedureStubbingRoot(std::shared_ptr < StubbingContext < R, arglist... >> stubbingContext) :
			MethodStubbingBase(stubbingContext),
			ProcedureStubbingProgress(),
			FirstProcedureStubbingProgress()
		{
		}

		ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

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
			startStubbing();
			initInvocationMockIfNeeded();
			return ProcedureStubbingProgress::Do(method);
		}

		virtual void VerifyInvocations(const int times) override {
			MethodStubbingBase::VerifyInvocations(times);
		}

 		virtual void startVerification() override {
			MethodStubbingBase::startVerification();
		}

		virtual void clearProgress() override {
			MethodStubbingBase::clearProgress();
		}
	};
}
#endif // ClousesImpl_h__