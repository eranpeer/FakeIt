/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef SortInvocations_hpp_
#define SortInvocations_hpp_

#include <set>

#include "fakeit/Invocation.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/SortInvocations.hpp"

namespace fakeit {

static void sortByInvocationOrder(std::unordered_set<Invocation*>& ivocations, std::vector<Invocation*>& result) {
	auto comparator = [](Invocation* a, Invocation* b)-> bool {return a->getOrdinal() < b->getOrdinal();};
	std::set<Invocation*, bool (*)(Invocation* a, Invocation* b)> sortedIvocations(comparator);
	for (auto i : ivocations)
		sortedIvocations.insert(i);

	for (auto i : sortedIvocations)
		result.push_back(i);
}

template<typename T>
static void collectInvocationSources(T&) {
}

template<typename ... list>
static void collectInvocationSources(std::set<const ActualInvocationsSource*>& into, const ActualInvocationsSource& mock,
		const list&... tail) {
	into.insert(&mock);
	collectInvocationSources(into, tail...);
}

template<typename ... list>
static void collectActualInvocations(std::unordered_set<Invocation*>& actualInvocations,
		std::set<const ActualInvocationsSource*>& invocationSources) {
	for (auto source : invocationSources) {
		source->getActualInvocations(actualInvocations);
	}
}

static void selectNonVerifiedInvocations(std::unordered_set<Invocation*>& actualInvocations, std::unordered_set<Invocation*>& into) {
	for (auto invocation : actualInvocations) {
		if (!invocation->isVerified()) {
			into.insert(invocation);
		}
	}
}

}

#endif // SortInvocations_hpp_
