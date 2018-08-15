//
// Created by eran on 09/04/2015.
//

#pragma once

#include <vector>
#include <unordered_set>

namespace fakeit {
    struct MatchAnalysis {
        std::vector<Invocation *> actualSequence;
        std::vector<Invocation *> matchedInvocations;
        int count;

        void run(InvocationsSourceProxy &involvedInvocationSources, std::vector<Sequence *> &expectedPattern) {
            getActualInvocationSequence(involvedInvocationSources, actualSequence);
            count = countMatches(expectedPattern, actualSequence, matchedInvocations);
        }

    private:
        static void getActualInvocationSequence(InvocationsSourceProxy &involvedMocks,
                                                std::vector<Invocation *> &actualSequence) {
            std::unordered_set<Invocation *> actualInvocations;
            collectActualInvocations(involvedMocks, actualInvocations);
            InvocationUtils::sortByInvocationOrder(actualInvocations, actualSequence);
        }

        static int countMatches(std::vector<Sequence *> &pattern, std::vector<Invocation *> &actualSequence,
                                std::vector<Invocation *> &matchedInvocations) {
            int end = -1;
            int count = 0;
            int startSearchIndex = 0;
            while (findNextMatch(pattern, actualSequence, startSearchIndex, end, matchedInvocations)) {
                count++;
                startSearchIndex = end;
            }
            return count;
        }

        static void collectActualInvocations(InvocationsSourceProxy &involvedMocks,
                                             std::unordered_set<Invocation *> &actualInvocations) {
            involvedMocks.getActualInvocations(actualInvocations);
        }

        static bool findNextMatch(std::vector<Sequence *> &pattern, std::vector<Invocation *> &actualSequence,
                                  int startSearchIndex, int &end,
                                  std::vector<Invocation *> &matchedInvocations) {
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


        static void collectMatchedInvocations(std::vector<Invocation *> &actualSequence,
                                              std::vector<Invocation *> &matchedInvocations, int start,
                                              int length) {
            int indexAfterMatchedPattern = start + length;
            for (; start < indexAfterMatchedPattern; start++) {
                matchedInvocations.push_back(actualSequence[start]);
            }
        }


        static bool isMatch(std::vector<Invocation *> &actualSequence,
                            std::vector<Invocation::Matcher *> &expectedSequence, int start) {
            bool found = true;
            for (unsigned int j = 0; found && j < expectedSequence.size(); j++) {
                Invocation *actual = actualSequence[start + j];
                Invocation::Matcher *expected = expectedSequence[j];
                found = found && expected->matches(*actual);
            }
            return found;
        }

        static int findNextMatch(Sequence *&pattern, std::vector<Invocation *> &actualSequence, int startSearchIndex) {
            std::vector<Invocation::Matcher *> expectedSequence;
            pattern->getExpectedSequence(expectedSequence);
            for (int i = startSearchIndex; i < ((int) actualSequence.size() - (int) expectedSequence.size() + 1); i++) {
                if (isMatch(actualSequence, expectedSequence, i)) {
                    return i;
                }
            }
            return -1;
        }

    };
}
