/*
 * SortInvocations.hpp
 *
 *  Created on: Feb 14, 2014
 *      Author: Eran
 */

#ifndef SortInvocations_hpp_
#define SortInvocations_hpp_

#include <set>

#include "fakeit/Invocation.hpp"

namespace fakeit {

static void sortByInvocationOrder(std::unordered_set<Invocation*>& ivocations, std::vector<Invocation*>& result) {
	auto comparator = [](Invocation* a, Invocation* b)-> bool {return a->getOrdinal() < b->getOrdinal();};
	std::set<Invocation*, bool (*)(Invocation* a, Invocation* b)> sortedIvocations(comparator);
	for (auto i : ivocations)
		sortedIvocations.insert(i);

	for (auto i : sortedIvocations)
		result.push_back(i);
}
}

#endif // SortInvocations_hpp_
