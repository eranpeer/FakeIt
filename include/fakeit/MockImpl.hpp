/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <type_traits>
#include <unordered_set>

#include <memory>

#include "mockutils/type_utils.hpp"
#include "mockutils/DynamicProxy.hpp"
#include "fakeit/StubbingImpl.hpp"
#include "fakeit/MethodMockingContext.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/FakeitContext.hpp"
#include "fakeit/ActualInvocationHandler.hpp"

namespace fakeit {

    
    template<typename C, typename ... baseclasses>
    class MockImpl : private MockObject<C>, public virtual ActualInvocationsSource {
    public:

        MockImpl(FakeitContext &fakeit, C &obj)
                : MockImpl<C, baseclasses...>(fakeit, obj, true) {
        }

        MockImpl(FakeitContext &fakeit)
                : MockImpl<C, baseclasses...>(fakeit, *(createFakeInstance()), false) {
            FakeObject<C, baseclasses...> *fake = reinterpret_cast<FakeObject<C, baseclasses...> *>(_instance);
            fake->getVirtualTable().setCookie(1, this);
        }

        virtual ~MockImpl() NO_THROWS {
            _proxy.detach();
            if (_isOwner) {
                FakeObject<C, baseclasses...> *fake = reinterpret_cast<FakeObject<C, baseclasses...> *>(_instance);
                delete fake;
            }
        }

        void detach() {
            _isOwner = false;
            _proxy.detach();
        }

        /**
         * Return all actual invocations of this mock.
         */
        void getActualInvocations(std::unordered_set<Invocation *> &into) const override {
            std::vector<ActualInvocationsSource *> vec;
            _proxy.getMethodMocks(vec);
            for (ActualInvocationsSource *s : vec) {
                s->getActualInvocations(into);
            }
        }

	    void initDataMembersIfOwner()
	    {
		    if (_isOwner) {
			    FakeObject<C, baseclasses...> *fake = reinterpret_cast<FakeObject<C, baseclasses...> *>(_instance);
			    fake->initializeDataMembersArea();
		    }
	    }

	    void reset() {
            _proxy.Reset();
		    initDataMembersIfOwner();
	    }

		void clear()
        {
			std::vector<ActualInvocationsContainer *> vec;
			_proxy.getMethodMocks(vec);
			for (ActualInvocationsContainer *s : vec) {
				s->clear();
			}
			initDataMembersIfOwner();
        }

        virtual C &get() override {
            return _proxy.get();
        }

        virtual FakeitContext &getFakeIt() override {
            return _fakeit;
        }

        template<class DATA_TYPE, typename T, typename ... arglist, class = typename std::enable_if<std::is_base_of<T, C>::value>::type>
        DataMemberStubbingRoot<C, DATA_TYPE> stubDataMember(DATA_TYPE T::*member, const arglist &... ctorargs) {
            _proxy.stubDataMember(member, ctorargs...);
            return DataMemberStubbingRoot<T, DATA_TYPE>();
        }

        template<int id, typename R, typename T, typename ... arglist, class = typename std::enable_if<std::is_base_of<T, C>::value>::type>
        MockingContext<R, arglist...> stubMethod(R(T::*vMethod)(arglist...)) {
            return MockingContext<R, arglist...>(new UniqueMethodMockingContextImpl < id, R, arglist... >
                   (*this, vMethod));
        }

        DtorMockingContext stubDtor() {
            return DtorMockingContext(new DtorMockingContextImpl(*this));
        }

    private:
        DynamicProxy<C, baseclasses...> _proxy;
        C *_instance; //
        bool _isOwner;
        FakeitContext &_fakeit;

        template<typename R, typename ... arglist>
        class MethodMockingContextBase : public MethodMockingContext<R, arglist...>::Context {
        protected:
            MockImpl<C, baseclasses...> &_mock;

            virtual RecordedMethodBody<R, arglist...> &getRecordedMethodBody() = 0;

        public:
            MethodMockingContextBase(MockImpl<C, baseclasses...> &mock) : _mock(mock) { }

            virtual ~MethodMockingContextBase() = default;

            void addMethodInvocationHandler(typename ActualInvocation<arglist...>::Matcher *matcher,
                ActualInvocationHandler<R, arglist...> *invocationHandler) {
                getRecordedMethodBody().addMethodInvocationHandler(matcher, invocationHandler);
            }

            void scanActualInvocations(const std::function<void(ActualInvocation<arglist...> &)> &scanner) {
                getRecordedMethodBody().scanActualInvocations(scanner);
            }

            void setMethodDetails(std::string mockName, std::string methodName) {
                getRecordedMethodBody().setMethodDetails(mockName, methodName);
            }

            bool isOfMethod(MethodInfo &method) {
                return getRecordedMethodBody().isOfMethod(method);
            }

            ActualInvocationsSource &getInvolvedMock() {
                return _mock;
            }

            std::string getMethodName() {
                return getRecordedMethodBody().getMethod().name();
            }

        };

        template<typename R, typename ... arglist>
        class MethodMockingContextImpl : public MethodMockingContextBase<R, arglist...> {
        protected:

            R (C::*_vMethod)(arglist...);
            
        public:
            virtual ~MethodMockingContextImpl() = default;

            MethodMockingContextImpl(MockImpl<C, baseclasses...> &mock, R (C::*vMethod)(arglist...))
                    : MethodMockingContextBase<R, arglist...>(mock), _vMethod(vMethod) {
            }

            
            virtual std::function<R(arglist&...)> getOriginalMethod() override {
                void *mPtr = MethodMockingContextBase<R, arglist...>::_mock.getOriginalMethod(_vMethod);
                C * instance = &(MethodMockingContextBase<R, arglist...>::_mock.get());
                return [=](arglist&... args) -> R {
                    auto m = union_cast<typename VTableMethodType<R,arglist...>::type>(mPtr);
                    return m(instance, std::forward<arglist>(args)...);
                };
            }
        };


        template<int id, typename R, typename ... arglist>
        class UniqueMethodMockingContextImpl : public MethodMockingContextImpl<R, arglist...> {
        protected:

            virtual RecordedMethodBody<R, arglist...> &getRecordedMethodBody() override {
                return MethodMockingContextBase<R, arglist...>::_mock.template stubMethodIfNotStubbed<id>(
                        MethodMockingContextBase<R, arglist...>::_mock._proxy,
                        MethodMockingContextImpl<R, arglist...>::_vMethod);
            }

        public:

            UniqueMethodMockingContextImpl(MockImpl<C, baseclasses...> &mock, R (C::*vMethod)(arglist...))
                    : MethodMockingContextImpl<R, arglist...>(mock, vMethod) {
            }
        };

        class DtorMockingContextImpl : public MethodMockingContextBase<void> {

        protected:

            virtual RecordedMethodBody<void> &getRecordedMethodBody() override {
                return MethodMockingContextBase<void>::_mock.stubDtorIfNotStubbed(
                        MethodMockingContextBase<void>::_mock._proxy);
            }

        public:
            virtual ~DtorMockingContextImpl() = default;

            DtorMockingContextImpl(MockImpl<C, baseclasses...> &mock)
                    : MethodMockingContextBase<void>(mock) {
            }

            virtual std::function<void()> getOriginalMethod() override {
                C &instance = MethodMockingContextBase<void>::_mock.get();
                return [=, &instance]() -> void {
                };
            }

        };

        static MockImpl<C, baseclasses...> *getMockImpl(void *instance) {
            FakeObject<C, baseclasses...> *fake = reinterpret_cast<FakeObject<C, baseclasses...> *>(instance);
            MockImpl<C, baseclasses...> *mock = reinterpret_cast<MockImpl<C, baseclasses...> *>(fake->getVirtualTable().getCookie(
                    1));
            return mock;
        }

        void unmocked() {
            ActualInvocation<> invocation(Invocation::nextInvocationOrdinal(), UnknownMethod::instance());
            UnexpectedMethodCallEvent event(UnexpectedType::Unmocked, invocation);
            auto &fakeit = getMockImpl(this)->_fakeit;
            fakeit.handle(event);

            std::string format = fakeit.format(event);
            UnexpectedMethodCallException e(format);
            throw e;
        }

        static C *createFakeInstance() {
            FakeObject<C, baseclasses...> *fake = new FakeObject<C, baseclasses...>();
            void *unmockedMethodStubPtr = union_cast<void *>(&MockImpl<C, baseclasses...>::unmocked);
            fake->getVirtualTable().initAll(unmockedMethodStubPtr);
            return reinterpret_cast<C *>(fake);
        }

        template<typename R, typename ... arglist>
        void *getOriginalMethod(R (C::*vMethod)(arglist...)) {
            auto vt = _proxy.getOriginalVT();
            auto offset = VTUtils::getOffset(vMethod);
            void *origMethodPtr = vt.getMethod(offset);
            return origMethodPtr;
        }

        void *getOriginalDtor() {
            auto vt = _proxy.getOriginalVT();
            auto offset = VTUtils::getDestructorOffset<C>();
            void *origMethodPtr = vt.getMethod(offset);
            return origMethodPtr;
        }

        template<unsigned int id, typename R, typename ... arglist>
        RecordedMethodBody<R, arglist...> &stubMethodIfNotStubbed(DynamicProxy<C, baseclasses...> &proxy,
                                                                  R (C::*vMethod)(arglist...)) {
            if (!proxy.isMethodStubbed(vMethod)) {
                proxy.template stubMethod<id>(vMethod, createRecordedMethodBody < R, arglist... > (*this, vMethod));
            }
            Destructible *d = proxy.getMethodMock(vMethod);
            RecordedMethodBody<R, arglist...> *methodMock = dynamic_cast<RecordedMethodBody<R, arglist...> *>(d);
            return *methodMock;
        }

        RecordedMethodBody<void> &stubDtorIfNotStubbed(DynamicProxy<C, baseclasses...> &proxy) {
            if (!proxy.isDtorStubbed()) {
                proxy.stubDtor(createRecordedDtorBody(*this));
            }
            Destructible *d = proxy.getDtorMock();
            RecordedMethodBody<void> *dtorMock = dynamic_cast<RecordedMethodBody<void> *>(d);
            return *dtorMock;
        }

        MockImpl(FakeitContext &fakeit, C &obj, bool isSpy)
                : _proxy{obj}, _instance(&obj), _isOwner(!isSpy), _fakeit(fakeit) {
        }

        template<typename R, typename ... arglist>
        static RecordedMethodBody<R, arglist...> *createRecordedMethodBody(MockObject<C> &mock,
                                                                           R(C::*vMethod)(arglist...)) {
            return new RecordedMethodBody<R, arglist...>(mock.getFakeIt(), typeid(vMethod).name());
        }

        static RecordedMethodBody<void> *createRecordedDtorBody(MockObject<C> &mock) {
            return new RecordedMethodBody<void>(mock.getFakeIt(), "dtor");
        }

    };
}
