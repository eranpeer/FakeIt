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
		public virtual ActualInvocationsSource,
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

	void getActualInvocations(std::unordered_set<AnyInvocation*>& into) const override {
		std::vector<std::shared_ptr<ActualInvocation<arglist...>>>actualInvocations = stubbingContext->getMethodMock().getActualInvocations(*invocationMatcher);
		for (auto i : actualInvocations) {
			AnyInvocation* ai = i.get();
			into.insert(ai);
		}
	}

	void getExpectedSequence(std::vector<AnyInvocationMatcher*>& into) const override {
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

static void sort(std::unordered_set<AnyInvocation*>& actualIvocations, std::vector<AnyInvocation*>& actualSequence) {
	auto comp = [](AnyInvocation* a, AnyInvocation* b)-> bool {return a->getOrdinal() < b->getOrdinal();};
	std::set<AnyInvocation*, bool (*)(AnyInvocation* a, AnyInvocation* b)> sortedActualIvocations(comp);
	for (auto i : actualIvocations)
		sortedActualIvocations.insert(i);

	for (auto i : sortedActualIvocations)
		actualSequence.push_back(i);
}

class VerifyFunctor {

	std::vector<Sequence*>& concat(std::vector<Sequence*>& vec) {
		return vec;
	}

	template<typename ... list>
	std::vector<Sequence*>& concat(std::vector<Sequence*>& vec, const Sequence& sequence, const list&... tail) {
		vec.push_back(&const_cast<Sequence&>(sequence));
		return concat(vec, tail...);
	}

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
			collectActualInvocations(expectedPattern, actualIvocations);

			std::vector<AnyInvocation*> actualSequence;
			sort(actualIvocations, actualSequence);

			std::vector<AnyInvocation*> matchedInvocations;
			int count = countMatches(expectedPattern, actualSequence, matchedInvocations);

			if (expectedInvocationCount == AT_LEAST_ONCE()) {
				if (count == 0) {
					throw(VerificationException(std::string("Expected invocation scenario does not match actual invocation order")));
				}
			} else if (count != expectedInvocationCount) {
				throw(VerificationException(
						std::string("expected ") + std::to_string(expectedInvocationCount) + " but was " + std::to_string(count)));
			}

			markAsVerified(matchedInvocations);
		}

	private:
		std::vector<Sequence*> expectedPattern;
		const Sequence& sequence;
		int expectedInvocationCount;
		bool _isActive;

		static inline int AT_LEAST_ONCE() {
			return -1;
		}

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

		void collectActualInvocations(std::vector<Sequence*>& expectedPattern, std::unordered_set<AnyInvocation*>& actualIvocations) {
			for (auto scenario : expectedPattern) {
				scenario->getActualInvocations(actualIvocations);
			}
		}
		virtual void verifyInvocations(const int times) override {
			expectedInvocationCount = times;
		}

		void markAsVerified(std::vector<AnyInvocation*>& matchedInvocations) {
			for (auto i : matchedInvocations)
				i->markAsVerified();
		}

		int countMatches(std::vector<Sequence*> &pattern, std::vector<AnyInvocation*>& actualSequence,
				std::vector<AnyInvocation*>& matchedInvocations) {
			int end = -1;
			int count = 0;
			int startSearchIndex = 0;
			while (findNextMatch(pattern, actualSequence, startSearchIndex, end, matchedInvocations)) {
				count++;
				startSearchIndex = end;
			}
			return count;
		}

		void collectMatchedInvocations(std::vector<AnyInvocation*>& actualSequence, std::vector<AnyInvocation*>& matchedInvocations,
				int start, int length) {
			int indexAfterMatchedPattern = start + length;
			for (; start < indexAfterMatchedPattern; start++) {
				matchedInvocations.push_back(actualSequence[start]);
			}
		}

		bool findNextMatch(std::vector<Sequence*> &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex, int& end,
				std::vector<AnyInvocation*>& matchedInvocations) {
			for (auto sequence : pattern) {
				int index = findNextMatch(sequence, actualSequence, startSearchIndex);
				if (index == -1) {
					return false;
				}
				collectMatchedInvocations(actualSequence, matchedInvocations, index, sequence->size());
				startSearchIndex = index + sequence->size();
			}
			end = startSearchIndex;
			return true;
		}

		bool isMatch(std::vector<AnyInvocation*>& actualSequence, std::vector<AnyInvocationMatcher*>& expectedSequence, int start) {
			bool found = true;
			for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
				AnyInvocation* actual = actualSequence[start + j];
				AnyInvocationMatcher* expected = expectedSequence[j];
				if (j >= 1) {
					AnyInvocation* prevActual = actualSequence[start + j - 1];
					found = actual->getOrdinal() - prevActual->getOrdinal() == 1;
				}
				found = found && expected->matches(*actual);
			}
			return found;
		}

		int findNextMatch(Sequence* &pattern, std::vector<AnyInvocation*>& actualSequence, int startSearchIndex) {
			std::vector<AnyInvocationMatcher*> expectedSequence;
			pattern->getExpectedSequence(expectedSequence);
			for (int i = startSearchIndex; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
				if (isMatch(actualSequence, expectedSequence, i)) {
					return i;
				}
			}
			return -1;
		}

	};

	VerifyFunctor() {
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

class VerifyNoOtherInvocationsFunctor {

	std::string format(AnyInvocation& i) {
		return {};
	}

	std::string buildNoOtherInvocationsVerificationErrorMsg( //
			std::vector<AnyInvocation*>& allIvocations, //
			std::vector<AnyInvocation*>& unverifedIvocations) {
		return std::string("found ") + std::to_string(unverifedIvocations.size()) + " non verified invocations";
	}

public:
	VerifyNoOtherInvocationsFunctor() {
	}

	void operator()() {
	}

	void selectNonVerifiedInvocations(std::unordered_set<AnyInvocation*>& actualInvocations, std::unordered_set<AnyInvocation*>& into) {
		for (auto invocation : actualInvocations) {
			if (!invocation->isVerified()) {
				into.insert(invocation);
			}
		}
	}

	template<typename ... list>
	void operator()(const ActualInvocationsSource& head, const list&... tail) {
		std::unordered_set<AnyInvocation*> actualInvocations;
		head.getActualInvocations(actualInvocations);

		std::unordered_set<AnyInvocation*> nonVerifedIvocations;
		selectNonVerifiedInvocations(actualInvocations, nonVerifedIvocations);

		if (nonVerifedIvocations.size() > 0) {
			std::vector<AnyInvocation*> sortedNonVerifedIvocations;
			sort(nonVerifedIvocations, sortedNonVerifedIvocations);

			std::vector<AnyInvocation*> sortedActualIvocations;
			sort(actualInvocations, sortedActualIvocations);

			throw VerificationException(buildNoOtherInvocationsVerificationErrorMsg(sortedActualIvocations, sortedNonVerifedIvocations));
		}
		return operator()(tail...);
	}
}
static VerifyNoOtherInvocations;

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
