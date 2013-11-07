#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>

#include "../mock4cpp/MethodMock.h"
#include "../mock4cpp/Stubbing.h"

namespace mock4cpp {

	namespace stubbing {

		static enum ProgressType
		{
			NONE, STUBBING, VERIFYING
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

	template <typename R, typename... arglist>
	class FunctionStubbingRoot : public virtual FirstFunctionStubbingProgress<R, arglist...>,
		private virtual FunctionStubbingProgress<R, arglist...>,
		public virtual verification::FunctionVerificationProgress {
	private:
		MethodMock<R, arglist...>& methodMock;
		FunctionStubbingRoot & operator= (const FunctionStubbingRoot & other) = delete;
		MethodInvocationMockBase<R, arglist...>* invocationMock;
		ProgressType progressType;
		int expectedInvocationCount;

		int CountInvocations(MethodInvocationMockBase<R, arglist...> &invocationMock) {
			int times = methodMock.getActualInvocations(invocationMock).size();
			return times;
		}

		void initInvocationMockIfNeeded(){
			if (!invocationMock){
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}
		}

	protected:

		virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override  {
			return *invocationMock;
		}

	public:
		FunctionStubbingRoot(MethodMock<R, arglist...>& methodMock) :
			FunctionStubbingProgress(),
			FirstFunctionStubbingProgress(),
			methodMock(methodMock),
			invocationMock(nullptr),
			progressType(ProgressType::NONE),
			expectedInvocationCount(-1)
		{
		}

		FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

		virtual ~FunctionStubbingRoot() override {
			if (progressType == ProgressType::NONE){
				return;
			}

			initInvocationMockIfNeeded();

			if (progressType == ProgressType::STUBBING){
				methodMock.stubMethodInvocation(invocationMock);
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
			progressType = ProgressType::STUBBING;
			initInvocationMockIfNeeded();
			return FunctionStubbingProgress::Do(method);
		}

		virtual void VerifyInvocations(const int times) override {
			progressType = ProgressType::VERIFYING;
			expectedInvocationCount = times;
		}

		void startStubbing() {
			progressType = ProgressType::STUBBING;
		}

		virtual void startVerification() {
			progressType = ProgressType::VERIFYING;
		}

		virtual void clearProgress() {
			progressType = ProgressType::NONE;
			if (invocationMock) {
				delete invocationMock;
				invocationMock = nullptr;
			}
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
		ProgressType progressType;
		int expectedInvocationCount;

		int CountInvocations(MethodInvocationMockBase<R, arglist...> &invocationMock) {
			int times = methodMock.getActualInvocations(invocationMock).size();
			return times;
		}

		void initInvocationMockIfNeeded(){
			if (!invocationMock){
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}
		}
	protected:
		virtual MethodInvocationMockBase<R, arglist...>& InvocationMock() override  {
			return *invocationMock;
		}

	public:
		ProcedureStubbingRoot(MethodMock<R, arglist...>& methodMock) :
			ProcedureStubbingProgress(),
			FirstProcedureStubbingProgress(),
			methodMock(methodMock),
			invocationMock(nullptr),
			progressType(ProgressType::NONE),
			expectedInvocationCount(-1)
		{
		}

		ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

		virtual ~ProcedureStubbingRoot() override {
			if (progressType == ProgressType::NONE) {
				return;
			}

			initInvocationMockIfNeeded();

			if (progressType == ProgressType::STUBBING){
				methodMock.stubMethodInvocation(invocationMock);
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
			progressType = ProgressType::STUBBING;
			initInvocationMockIfNeeded();
			return ProcedureStubbingProgress::Do(method);
		}

		virtual void VerifyInvocations(const int times) override {
			progressType = ProgressType::VERIFYING;
			expectedInvocationCount = times;
		}

		void startStubbing(){
			progressType = ProgressType::STUBBING;
		}

		virtual void startVerification() {
			progressType = ProgressType::VERIFYING;
		}

		virtual void clearProgress() {
			progressType = ProgressType::NONE;
			if (invocationMock) {
				delete invocationMock;
				invocationMock = nullptr;
			}
		}
	};

}
#endif // ClousesImpl_h__