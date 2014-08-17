/*
* Copyright (c) 2014 Eran Pe'er.
*
* This program is made available under the terms of the MIT License.
*
* Created on Mar 10, 2014
*/

#ifndef MockImpl_h__
#define MockImpl_h__

#include <type_traits>
#include <unordered_set>

#include <memory>

#include "mockutils/DynamicProxy.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/FakeIt.hpp"

namespace fakeit {

	template<typename C, typename ... baseclasses>
	class MockImpl : public MockObject<C>, public virtual ActualInvocationsSource {
	public:

		MockImpl(C &obj) :MockImpl<C, baseclasses...>(obj, true) {
		}

		MockImpl() : MockImpl<C, baseclasses...>(*(createFakeInstance()), false) {
		}

		virtual ~MockImpl() {
			if (_isSpy)
				return;
			FakeObject<C, baseclasses...>* fake = (FakeObject<C, baseclasses...>*) _instance;
			delete fake;
		}

		/**
		* Return all actual invocations of this mock.
		*/
		void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
			std::vector<ActualInvocationsSource*> vec;
			_proxy.getMethodMocks(vec);
			for (ActualInvocationsSource * s : vec) {
				s->getActualInvocations(into);
			}
		}

		void reset() {
			_proxy.Reset();
			if (!_isSpy) {
				FakeObject<C, baseclasses...>* fake = (FakeObject<C, baseclasses...>*) _instance;
				fake->initializeDataMembersArea();
			}
		}

		virtual C& get() override {
			return _proxy.get();
		}

		template<class DATA_TYPE, typename ... arglist>
		DataMemberStubbingRoot<C, DATA_TYPE> stubDataMember(DATA_TYPE C::*member, const arglist&... ctorargs) {
			_proxy.stubDataMember(member, ctorargs...);
			return DataMemberStubbingRoot<C, DATA_TYPE>();
		}

		template<typename R, typename ... arglist, class = typename std::enable_if<!std::is_void<R>::value>::type>
		FunctionStubbingRoot<C, R, arglist...> stub(R(C::*vMethod)(arglist...)) {
			return FunctionStubbingRoot<C, R, arglist...>(
				std::shared_ptr<MethodStubbingContext<C, R, arglist...>>(new MethodStubbingContextImpl<R, arglist...>(*this, vMethod)));
		}

		template<typename R, typename ... arglist, class = typename std::enable_if<std::is_void<R>::value>::type>
		ProcedureStubbingRoot<C, R, arglist...> stub(R(C::*vMethod)(arglist...)) {
			return ProcedureStubbingRoot<C, R, arglist...>(
				std::shared_ptr<MethodStubbingContext<C, R, arglist...>>(new MethodStubbingContextImpl<R, arglist...>(*this, vMethod)));
		}

	private:
		DynamicProxy<C, baseclasses...> _proxy;
		C* _instance; //
		bool _isSpy;

		template<typename R, typename ... arglist>
		class MethodStubbingContextImpl : public MethodStubbingContext<C, R, arglist...> {
			MockImpl<C, baseclasses...>& mock;
			typename MethodStubbingContext<C, R, arglist...>::MethodType vMethod;
		public:

			MethodStubbingContextImpl(MockImpl<C, baseclasses...>& mock, R(C::*vMethod)(arglist...)) :
				mock(mock), vMethod(vMethod) {
			}

			virtual MethodMock<C, R, arglist...>& getMethodMock() override {
				return mock.stubMethodIfNotStubbed(mock._proxy, vMethod);
			}

			virtual void getActualInvocations(std::unordered_set<Invocation*>& into) const override {
				mock.getActualInvocations(into);
			}

			virtual typename MethodStubbingContext<C, R, arglist...>::MethodType getOriginalMethod() override {
				void * mPtr = mock.getOriginalMethod(vMethod);
				return union_cast<typename MethodStubbingContext<C, R, arglist...>::MethodType>(mPtr);
			}

			virtual MockObject<C>& getMock() override {
				return mock;
			}

		};

		void unmocked() {
			struct UnmockedMethodInvocation : public UnexpectedMethodCallException {
				UnmockedMethodInvocation() :UnexpectedMethodCallException( //
				std::shared_ptr<Invocation> {new ActualInvocation<>(nextInvocationOrdinal(), UnknownMethod::instance())} //
				){} //
			} e;
			FakeIt::getInstance().handle(e);
			throw e;
		}

		static C* createFakeInstance() {
			FakeObject<C, baseclasses...>* fake = new FakeObject<C, baseclasses...>();
			fake->initializeDataMembersArea();
			void* unmockedMethodStubPtr = union_cast<void*>(&MockImpl<C, baseclasses...>::unmocked);
			fake->getVirtualTable().initAll(unmockedMethodStubPtr);
			return (C*) fake;
		}

		template<typename R, typename ... arglist>
		void * getOriginalMethod(R(C::*vMethod)(arglist...)) {
			auto vt = _proxy.getOriginalVT();
			auto offset = VTUtils::getOffset(vMethod);
			void * origMethodPtr = vt.getMethod(offset);
			return origMethodPtr;
		}

		template<typename R, typename ... arglist>
		MethodMock<C, R, arglist...>& stubMethodIfNotStubbed(DynamicProxy<C, baseclasses...> &proxy, R(C::*vMethod)(arglist...)) {
			if (!proxy.isStubbed(vMethod)) {
				auto methodMock = std::shared_ptr<MethodInvocationHandler<R, arglist...>> { new MethodMock<C, R, arglist...>(*this, vMethod) };
				proxy.stubMethod(vMethod, methodMock);
			}
			Destructable * d = proxy.getMethodMock(vMethod);
			MethodMock<C, R, arglist...> * methodMock = dynamic_cast<MethodMock<C, R, arglist...> *>(d);
			return *methodMock;
		}

		MockImpl(C &obj, bool isSpy) :
			_proxy{ obj }, _instance(&obj), _isSpy(isSpy) {
		}
	};
}

#endif