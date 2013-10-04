#ifndef mockito_clouses_h__
#define mockito_clouses_h__

namespace mock4cpp {

	namespace mockito {

		namespace clouses {

			template <typename R>
			struct NextFunctionWhenClouse {

				virtual ~NextFunctionWhenClouse() {};

				template<typename NO_REF = std::remove_reference<R>::type>
				typename std::enable_if<is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
					ThenReturn(const R& r) {
						return ThenDo(std::function<R()>([r](...)->R{
							return r;
						}));
					}

				template<typename NO_REF = std::remove_reference<R>::type>
				typename std::enable_if<!is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
					ThenReturn(const R& r) {
						return ThenDo(std::function<R()>([&r](...)->R{ return r; }));
					}

				template <typename E>
				NextFunctionWhenClouse<R>& ThenThrow(const E& e) {
					return ThenDo(std::function<R()>([e](...)->R{throw e; }));
				}

				NextFunctionWhenClouse<R>& ThenDo(R(*method)()) {
					return ThenDo(std::function<R()>(method));
				}

				virtual  NextFunctionWhenClouse<R>& ThenDo(std::function<R()> method) = 0;

			};


			template <typename R>
			struct FirstFunctionWhenClouse {
				virtual ~FirstFunctionWhenClouse() = 0 {};

				template<typename NO_REF = std::remove_reference<R>::type>
				typename std::enable_if<std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
					Return(const R& r) {
						return Do(std::function<R()>([r](...)->R{ return r; }));
					}

				template<typename NO_REF = std::remove_reference<R>::type>
				typename std::enable_if<!std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R>&>::type
					Return(const R& r) {
						return Do(std::function<R()>([&r](...)->R{ return r; }));
					}

				template <typename E>
				NextFunctionWhenClouse<R>& Throw(const E& e)  {
					return Do(std::function<R()>([e](...)->R{throw e; }));
				}

				virtual  NextFunctionWhenClouse<R>& Do(std::function<R()> method) = 0;

			};

			template <typename R>
			struct NextProcedureWhenClouse {
				virtual ~NextProcedureWhenClouse() = 0 {};

				NextProcedureWhenClouse<R>& ThenReturn() {
					return ThenDo(std::function<R()>([](...)->R{ return DefaultValue::value<R>(); }));
				}

				template <typename E>
				NextProcedureWhenClouse<R>& ThenThrow(const E e) {
					return ThenDo(std::function<R()>([e](...)->R{ throw e; }));
				}

				virtual  NextProcedureWhenClouse<R>& ThenDo(std::function<R()> method) = 0;
			};


			template <typename R>
			struct FirstProcedureWhenClouse {

				virtual ~FirstProcedureWhenClouse() = 0 {};

				NextProcedureWhenClouse<R>& Return() {
					return Do(std::function<R()>([](...)->R{ return DefaultValue::value<R>(); }));
				}

				template <typename E>
				NextProcedureWhenClouse<R>& Throw(const E e) {
					return Do(std::function<R()>([e](...)->R{ throw e; }));
				}

				virtual  NextProcedureWhenClouse<R>& Do(std::function<R()> method) = 0;
			};

		}

	}






	namespace mockito {


		template < typename R, typename... arglist>
		struct BehaviorMock
		{
			virtual R invoke(const arglist&... args) = 0;
		};

		template <typename R, typename... arglist>
		struct DoMock : public BehaviorMock<R, arglist...>
		{
			DoMock(std::function<R(arglist...)> f) :f(f){}
			virtual R invoke(const arglist&... args) override {
				return f(args...);
			}
		private:
			std::function<R(arglist...)> f;
		};

		template <typename R, typename... arglist>
		struct MethodInvocationMock : public ActualInvocation
		{
			void append(BehaviorMock<R, arglist...>* mock){
				behaviorMocks.push_back(mock);
			}

			void appendDo(std::function<R(arglist...)> method) {
				auto doMock = new DoMock<R, arglist...>(method);
				append(doMock);
			}

			void clear(){
				behaviorMocks.clear();
			}

			virtual bool matchesActual(const arglist&... args) = 0;

			virtual bool matchesExpected(const arglist&... args) = 0;

			R handleMethodInvocation(const arglist&... args){
				BehaviorMock<R, arglist...>* behavior = behaviorMocks.front();
				if (behaviorMocks.size() > 1)
					behaviorMocks.erase(behaviorMocks.begin());
				return behavior->invoke(args...);
			}

		private:
			std::vector<BehaviorMock<R, arglist...>*> behaviorMocks;
		};

		template <typename R, typename... arglist>
		struct SimpleMethodInvocationMock : public MethodInvocationMock<R, arglist...>
		{
			SimpleMethodInvocationMock(const arglist&... args) : expectedArguments(args...)
			{
			}

			virtual bool matchesActual(const arglist&... args) override {
				return expectedArguments == std::tuple<arglist...>(args...);
			}

			virtual bool matchesExpected(const arglist&... args) override {
				return expectedArguments == std::tuple<arglist...>(args...);
			}

		private:
			const std::tuple <arglist...> expectedArguments;
		};

		template <typename R, typename... arglist>
		struct DefaultMethodInvocationMock : public MethodInvocationMock<R, arglist...>
		{
			DefaultMethodInvocationMock(std::function<R(arglist...)> methodBehavior) {
				appendDo(methodBehavior);
			}

			virtual bool matchesActual(const arglist&... args) override {
				return true;
			}

			virtual bool matchesExpected(const arglist&... args) override {
				return false;
			}

		};

		template <typename R, typename... arglist>
		struct MethodMock : public MethodInvocationHandler <R, arglist...>
		{
			MethodMock(MockBase& mock) :mock{ mock }
			{}

			virtual ~MethodMock() override {}

			void addMethodCall(MethodInvocationMock<R, arglist...> * mock){
				methodInvocationMocks.push_back(mock);
			}

			void clear(){
				methodInvocationMocks.clear();
			}

			MethodInvocationMock<R, arglist...>* last(){
				return methodInvocationMocks.back();
			}

			R handleMethodInvocation(const arglist&... args) override {
				auto * methodInvocationMock = getMethodInvocationMockForActualArgs(args...);
				mock.addActualInvocation(methodInvocationMock);
				return methodInvocationMock->handleMethodInvocation(args...);
			}

			MethodInvocationMock<R, arglist...> * stubMethodCall(const arglist&... args){
				MethodInvocationMock<R, arglist...> * methodInvocationMock = getMethodInvocationMockForExpectedArgs(args...);
				if (methodInvocationMock == nullptr) {
					methodInvocationMock = new SimpleMethodInvocationMock<R, arglist...>(args...);
					addMethodCall(methodInvocationMock);
				}
				return methodInvocationMock;
			}

		private:

			MockBase& mock;
			std::vector<MethodInvocationMock<R, arglist...>*> methodInvocationMocks;

			MethodInvocationMock<R, arglist...> * getMethodInvocationMockForExpectedArgs(const arglist&... expectedArgs){
				for (auto i = methodInvocationMocks.rbegin(); i != methodInvocationMocks.rend(); ++i) {
					if ((*i)->matchesExpected(expectedArgs...)){
						return (*i);
					}
				}
				return nullptr;
			}

			MethodInvocationMock<R, arglist...>* getMethodInvocationMockForActualArgs(const arglist&... args) {
				for (auto i = methodInvocationMocks.rbegin(); i != methodInvocationMocks.rend(); ++i) {
					if ((*i)->matchesActual(args...)){
						return (*i);
					}
				}
				// should not get here since the default will always match an actual method call.
				return nullptr;
			}

		};

	}

}
#endif // mockito_clouses_h__