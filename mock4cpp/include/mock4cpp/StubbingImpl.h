#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>

#include "mock4cpp/MethodMock.h"
#include "mock4cpp/Stubbing.h"
#include "mock4cpp/ActualInvocation.h"
#include "mock4cpp/InvocationMatcher.h"
#include "mockutils/ExtractMemberType.h"

namespace mock4cpp {

enum class ProgressType {
	NONE, STUBBING, VERIFYING
};

template<typename R, typename ... arglist>
struct StubbingContext {
	virtual ~StubbingContext() {
	}
	virtual MethodMock<R, arglist...>& getMethodMock() = 0;
};

template<typename R, typename ... arglist>
struct FunctionStubbingProgress: protected virtual FirstFunctionStubbingProgress<R, arglist...>, //
		protected virtual NextFunctionStubbingProgress<R, arglist...> {

	FunctionStubbingProgress() = default;
	virtual ~FunctionStubbingProgress() override = default;

	NextFunctionStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		recordedMethodBody().appendDo(method);
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		recordedMethodBody().clear();
		recordedMethodBody().appendDo(method);
		return *this;
	}

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() = 0;
private:
	FunctionStubbingProgress & operator=(const FunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct ProcedureStubbingProgress: protected virtual FirstProcedureStubbingProgress<R, arglist...>, //
		protected virtual NextProcedureStubbingProgress<R, arglist...> {

	ProcedureStubbingProgress() = default;
	~ProcedureStubbingProgress() override = default;

	NextProcedureStubbingProgress<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		recordedMethodBody().appendDo(method);
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		recordedMethodBody().clear();
		recordedMethodBody().appendDo(method);
		return *this;
	}

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() = 0;
private:
	ProcedureStubbingProgress & operator=(const ProcedureStubbingProgress & other) = delete;
};

using namespace mock4cpp;

template<typename R, typename ... arglist>
class MethodStubbingBase: //
protected virtual MethodStubbingInternal,
		public virtual Sequence,
		protected virtual AnyInvocationMatcher {
private:

	std::shared_ptr<RecordedMethodBody<R, arglist...>> buildInitialMethodBody() {
		auto initialMethodBehavior = [](const arglist&... args)->R {return DefaultValue::value<R>();};
		std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody { new RecordedMethodBody<R, arglist...>() };
		recordedMethodBody->appendDo(initialMethodBehavior);
		return recordedMethodBody;
	}

protected:
	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

	std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext;
	std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher;
	std::shared_ptr<RecordedMethodBody<R, arglist...>> recordedMethodBody;

	ProgressType progressType;
	int expectedInvocationCount;

	MethodStubbingBase(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			stubbingContext(stubbingContext), invocationMatcher { new DefaultInvocationMatcher<arglist...>() }, progressType(
					ProgressType::NONE), expectedInvocationCount(-1) {
		recordedMethodBody = buildInitialMethodBody();
	}

	void setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> invocationMatcher) {
		MethodStubbingBase<R, arglist...>::invocationMatcher = invocationMatcher;
	}

	virtual ~MethodStubbingBase() THROWS {
		if (std::uncaught_exception()) {
			return;
		}

		if (progressType == ProgressType::NONE) {
			return;
		}

		if (progressType == ProgressType::STUBBING) {
			stubbingContext->getMethodMock().stubMethodInvocation(invocationMatcher, recordedMethodBody);
			return;
		}
	}

	virtual void startStubbing() {
		progressType = ProgressType::STUBBING;
	}

public:
	virtual bool matches(AnyInvocation& invocation) override {
		if (&invocation.getMethod() != &stubbingContext->getMethodMock()) {
			return false;
		}

		ActualInvocation<arglist...>& actualInvocation = dynamic_cast<ActualInvocation<arglist...>&>(invocation);
		return invocationMatcher->matches(actualInvocation);
	}

	void getActualInvocationSequence(std::unordered_set<AnyInvocation*>& into) const override {
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>>actualInvocations = stubbingContext->getMethodMock().getActualInvocations(*invocationMatcher);
		for (auto i : actualInvocations) {
			AnyInvocation* ai = i.get();
			into.insert(ai);
		}
	}

	void getExpectedInvocationSequence(std::vector<AnyInvocationMatcher*>& into) const override {
		const AnyInvocationMatcher* b = this;
		AnyInvocationMatcher* c = const_cast<AnyInvocationMatcher*>(b);
		into.push_back(c);
	}

	unsigned int size() const override {
		return 1;
	}

};

template<typename R, typename ... arglist>
class FunctionStubbingRoot: //
public virtual MethodStubbingBase<R, arglist...>, //
		private virtual FunctionStubbingProgress<R, arglist...> {
private:
	FunctionStubbingRoot & operator=(const FunctionStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;
protected:

	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() override {
		return *MethodStubbingBase<R, arglist...>::recordedMethodBody;
	}

	virtual void startStubbing() override {
		MethodStubbingBase<R, arglist...>::startStubbing();
	}

public:

	FunctionStubbingRoot(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			MethodStubbingBase<R, arglist...>(stubbingContext), FirstFunctionStubbingProgress<R, arglist...>(), FunctionStubbingProgress<R,
					arglist...>() {
	}

	FunctionStubbingRoot(const FunctionStubbingRoot& other) = default;

	~FunctionStubbingRoot() THROWS {
	}

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstFunctionStubbingProgress<R, arglist...>::operator =(method);
	}

	FunctionStubbingRoot<R, arglist...>& Using(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	FunctionStubbingRoot<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	NextFunctionStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<R, arglist...>::startStubbing();
		return FunctionStubbingProgress<R, arglist...>::Do(method);
	}
};

template<typename R, typename ... arglist>
class ProcedureStubbingRoot: //
public virtual MethodStubbingBase<R, arglist...>,
		private virtual ProcedureStubbingProgress<R, arglist...> {
private:
	ProcedureStubbingRoot & operator=(const ProcedureStubbingRoot & other) = delete;

	friend class VerifyFunctor;
	friend class StubFunctor;
	friend class WhenFunctor;

protected:
	virtual RecordedMethodBody<R, arglist...>& recordedMethodBody() override {
		return *MethodStubbingBase<R, arglist...>::recordedMethodBody;
	}

	virtual void startStubbing() override {
		MethodStubbingBase<R, arglist...>::startStubbing();
	}

public:
	ProcedureStubbingRoot(std::shared_ptr<StubbingContext<R, arglist...>> stubbingContext) :
			MethodStubbingBase<R, arglist...>(stubbingContext), FirstProcedureStubbingProgress<R, arglist...>(), ProcedureStubbingProgress<
					R, arglist...>() {
	}

	~ProcedureStubbingRoot() THROWS {
	}

	ProcedureStubbingRoot(const ProcedureStubbingRoot& other) = default;

	virtual void operator=(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		FirstProcedureStubbingProgress<R, arglist...>::operator=(method);
	}

	ProcedureStubbingRoot<R, arglist...>& Using(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& Matching(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& operator()(const arglist&... args) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new ExpectedArgumentsInvocationMatcher<arglist...>(args...) });
		return *this;
	}

	ProcedureStubbingRoot<R, arglist...>& operator()(std::function<bool(arglist...)> matcher) {
		MethodStubbingBase<R, arglist...>::setInvocationMatcher(std::shared_ptr<InvocationMatcher<arglist...>> {
				new UserDefinedInvocationMatcher<arglist...>(matcher) });
		return *this;
	}

	NextProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		// Must override since the implementation in base class is privately inherited
		MethodStubbingBase<R, arglist...>::startStubbing();
		return ProcedureStubbingProgress<R, arglist...>::Do(method);
	}
};

template<typename C, typename DATA_TYPE>
class DataMemberStubbingRoot {
private:
	//DataMemberStubbingRoot & operator= (const DataMemberStubbingRoot & other) = delete;
public:
	DataMemberStubbingRoot(const DataMemberStubbingRoot& other) = default;
	DataMemberStubbingRoot() = default;

	void operator=(const DATA_TYPE& val) {
	}
};

class VerifyFunctor {
public:

	struct VerificationProgress: public virtual MethodVerificationProgress {
		friend class VerifyFunctor;

		~VerificationProgress() THROWS {

			if (std::uncaught_exception()) {
				return;
			}

			if (!_isActive) {
				return;
			}

			std::unordered_set<AnyInvocation*> actualIvocations;
			for (auto scenario : expectedPattern) {
				scenario->getActualInvocationSequence(actualIvocations);
			}

			auto comp = [](AnyInvocation* a, AnyInvocation* b)-> bool {return a->getOrdinal() < b->getOrdinal();};
			std::set<AnyInvocation*, bool (*)(AnyInvocation* a, AnyInvocation* b)> sortedActualIvocations(comp);
			for (auto i : actualIvocations)
				sortedActualIvocations.insert(i);

			std::vector<AnyInvocation*> actualSequence;
			for (auto i : sortedActualIvocations)
				actualSequence.push_back(i);

			int count = countMatches(expectedPattern, actualSequence);

			if (expectedInvocationCount == -1) {
				if (count == 0) {
					throw(MethodCallVerificationException(
							std::string("Expected invocation scenario does not match actual invocation order")));
				}
				return;
			}
			if (count != expectedInvocationCount) {
				throw(MethodCallVerificationException(
						std::string("expected ") + std::to_string(expectedInvocationCount) + " but was " + std::to_string(count)));
			}
		}

		virtual void verifyInvocations(const int times) override {
			expectedInvocationCount = times;
		}

	private:
		std::vector<Sequence*> expectedPattern;
		const Sequence& sequence;
		int expectedInvocationCount;
		bool _isActive;

		VerificationProgress(const Sequence& sequence) :
				sequence(sequence), expectedInvocationCount(-1), _isActive(true) {
			expectedPattern.push_back(&const_cast<Sequence&>(sequence));
		}

		VerificationProgress(const std::vector<Sequence*> expectedPattern) :
				expectedPattern(expectedPattern), sequence(*expectedPattern[0]), expectedInvocationCount(-1), _isActive(true) {
		}

		VerificationProgress(VerificationProgress& other) :
				expectedPattern(other.expectedPattern), sequence(other.sequence), expectedInvocationCount(other.expectedInvocationCount), _isActive(
						other._isActive) {
			other._isActive = false;
		}

		int countMatches(std::vector<Sequence*> &pattern, std::vector<AnyInvocation*>& actualSequence) {
			int end = -1;
			int count = 0;
			int startSearchIndex = 0;
			while (findNextMatch(pattern, actualSequence, startSearchIndex, end)) {
				count++;
				startSearchIndex = end;
			}
			return count;
		}

		bool findNextMatch(std::vector<Sequence*> &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex, int& end) {
			for (auto sequence : pattern) {
				int index = findNextMatch(sequence, actualSequence, startSearchIndex);
				if (index == -1) {
					return false;
				}
				startSearchIndex = index + sequence->size();
			}
			end = startSearchIndex;
			return true;
		}

		int findNextMatch(Sequence* &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex) {
			std::vector<AnyInvocationMatcher*> expectedSequence;
			pattern->getExpectedInvocationSequence(expectedSequence);

			for (int i = startSearchIndex; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
				bool found = true;
				for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
					AnyInvocation* actual = actualSequence[i + j];
					AnyInvocationMatcher* expected = expectedSequence[j];
					if (j >= 1) {
						AnyInvocation* prevActual = actualSequence[i + j - 1];
						found = actual->getOrdinal() - prevActual->getOrdinal() == 1;
					}
					found = found && expected->matches(*actual);
				}
				if (found) {
					return i;
				}
			}
			return -1;
		}

	};

	VerifyFunctor() {
	}

	std::vector<Sequence*>& concat(std::vector<Sequence*>& vec) {
		return vec;
	}

	template<typename ... list>
	std::vector<Sequence*>& concat(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		return concat(vec, tail...);
	}

	template<typename ... list>
	VerificationProgress operator()(const Sequence& sequence, const list&... tail) {
		std::vector<Sequence*> vec;
		concat(vec, sequence, tail...);
		VerificationProgress progress(vec);
		return progress;
	}

}
static Verify;

class WhenFunctor {
public:
	WhenFunctor() {
	}

	template<typename R, typename ... arglist>
	FirstProcedureStubbingProgress<R, arglist...>& operator()(const ProcedureStubbingRoot<R, arglist...>& stubbingProgress) {
		ProcedureStubbingRoot<R, arglist...>& rootWithoutConst = const_cast<ProcedureStubbingRoot<R, arglist...>&>(stubbingProgress);
		return dynamic_cast<FirstProcedureStubbingProgress<R, arglist...>&>(rootWithoutConst);
	}

	template<typename R, typename ... arglist>
	FirstFunctionStubbingProgress<R, arglist...>& operator()(const FunctionStubbingRoot<R, arglist...>& stubbingProgress) {
		FunctionStubbingRoot<R, arglist...>& rootWithoutConst = const_cast<FunctionStubbingRoot<R, arglist...>&>(stubbingProgress);
		return dynamic_cast<FirstFunctionStubbingProgress<R, arglist...>&>(rootWithoutConst);
	}

}static When;

class StubFunctor {
private:
	void operator()() {
	}
public:
	StubFunctor() {
	}

	template<typename H>
	void operator()(const H& head) {
		H& headWithoutConst = const_cast<H&>(head);
		headWithoutConst.startStubbing();
	}

	template<typename H, typename ... M>
	void operator()(const H& head, const M&... tail) {
		H& headWithoutConst = const_cast<H&>(head);
		headWithoutConst.startStubbing();
		this->operator()(tail...);
	}

}static Stub;

//template<typename R, typename ... arglist>
//inline MethodStubbingBase<R,arglist...>& operator<<(const MethodStubbingBase<R,arglist...>& root, const R& rv)
//{
//	return (MethodStubbingBase<R,arglist...>&)root;
//}

}

#endif // ClousesImpl_h__
