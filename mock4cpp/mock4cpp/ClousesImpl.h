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

			FunctionWhenClouse() = default;
			~FunctionWhenClouse() override = default;

			AbstractNextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				InvocationMock().appendDo(method);
				return *this;
			}

			AbstractNextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
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
		class StubFunctionClouse : public virtual AbstractFirstFunctionWhenClouse<R, arglist...>,
			private virtual FunctionWhenClouse<R, arglist...>{
		private:
			MethodMock<R, arglist...>& methodMock;
			StubFunctionClouse & operator= (const StubFunctionClouse & other) = delete;
			MethodInvocationMock<R, arglist...>* invocationMock;
		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() override  {
				return *invocationMock;
			}

		public:
			StubFunctionClouse(MethodMock<R, arglist...>& methodMock) : FunctionWhenClouse(), AbstractFirstFunctionWhenClouse(), methodMock(methodMock) {
				auto initialMethodBehavior = [](const arglist&... args)->R&{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			StubFunctionClouse(const StubFunctionClouse& other) = default;

			virtual ~StubFunctionClouse() override {
				methodMock.addMethodCall(invocationMock);
			};

			virtual void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

			AbstractFirstFunctionWhenClouse<R, arglist...>& When(const arglist&... args) {
				this->invocationMock = &methodMock.stubMethodCall(args...);
				return *this;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			AbstractNextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}
		};

		template <typename R, typename... arglist>
		struct ProcedureWhenClouse :
			public virtual AbstractFirstProcedureWhenClouse<R, arglist...>,
			public virtual AbstractNextProcedureWhenClouse<R, arglist...>
		{

			ProcedureWhenClouse() = default;
			~ProcedureWhenClouse() override = default;

			AbstractNextProcedureWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
				InvocationMock().appendDo(method);
				return *this;
			}

			AbstractNextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
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
		class StubProcedureClouse : public virtual AbstractFirstProcedureWhenClouse<R, arglist...>,
			private virtual ProcedureWhenClouse<R, arglist...>{
		private:
			MethodMock<R, arglist...>& methodMock;
			StubProcedureClouse & operator= (const StubProcedureClouse & other) = delete;
			MethodInvocationMock<R, arglist...>* invocationMock;
		protected:
			virtual MethodInvocationMock<R, arglist...>& InvocationMock() override  {
				return *invocationMock;
			}

		public:
			StubProcedureClouse(MethodMock<R, arglist...>& methodMock) : ProcedureWhenClouse(), AbstractFirstProcedureWhenClouse(), methodMock(methodMock) {
				auto initialMethodBehavior = [](const arglist&... args)->R{return DefaultValue::value<R>(); };
				invocationMock = new DefaultInvocationMock<R, arglist...>(initialMethodBehavior);
			}

			StubProcedureClouse(const StubProcedureClouse& other) = default;

			virtual ~StubProcedureClouse() override {
				methodMock.addMethodCall(invocationMock);
			};

			virtual void operator=(std::function<R(arglist...)> method) override {
				Do(method);
			}

			AbstractFirstProcedureWhenClouse<R, arglist...>& When(const arglist&... args) {
				this->invocationMock = &methodMock.stubMethodCall(args...);
				//invocationMock->appendDo([](...)->R{ return DefaultValue::value<R>();	});
				return *this;
			}

			VerifyClouse Verify(const arglist&... args)  {
				return VerifyClouse(methodMock.getActualInvocations(args...).size());
			}

			AbstractNextProcedureWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) {
				InvocationMock().clear();
				InvocationMock().appendDo(method);
				return *this;
			}
		};

	}
}
#endif // ClousesImpl_h__