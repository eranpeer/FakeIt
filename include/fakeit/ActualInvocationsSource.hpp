//
// Created by eran on 01/04/2015.
//

#include <unordered_set>
#include "fakeit/Invocation.hpp"

namespace fakeit {

    struct ActualInvocationsSource {
        virtual void getActualInvocations(std::unordered_set<fakeit::Invocation *> &into) const = 0;

        virtual ~ActualInvocationsSource() THROWS { };
    };

    struct InvocationsSourceProxy : public ActualInvocationsSource {

        InvocationsSourceProxy(ActualInvocationsSource *inner) :
                _inner(inner) {
        }

        void getActualInvocations(std::unordered_set<fakeit::Invocation *> &into) const override {
            _inner->getActualInvocations(into);
        }

    private:
        std::shared_ptr<ActualInvocationsSource> _inner;
    };

    struct UnverifiedInvocationsSource : public ActualInvocationsSource {

        UnverifiedInvocationsSource(InvocationsSourceProxy decorated) : _decorated(decorated) {
        }

        void getActualInvocations(std::unordered_set<fakeit::Invocation *> &into) const override {
            std::unordered_set<fakeit::Invocation *> all;
            _decorated.getActualInvocations(all);
            for (fakeit::Invocation *i : all) {
                if (!i->isVerified()) {
                    into.insert(i);
                }
            }
        }

    private:
        InvocationsSourceProxy _decorated;
    };

    struct AggregateInvocationsSource : public ActualInvocationsSource {

        AggregateInvocationsSource(std::vector<ActualInvocationsSource *> &sources) : _sources(sources) {
        }

        void getActualInvocations(std::unordered_set<fakeit::Invocation *> &into) const override {
            std::unordered_set<fakeit::Invocation *> tmp;
            for (ActualInvocationsSource *source : _sources) {
                source->getActualInvocations(tmp);
            }
            filter(tmp, into);
        }

    protected:
        bool shouldInclude(fakeit::Invocation *invocation) const {
            return true;
        }

    private:
        std::vector<ActualInvocationsSource *> _sources;

        void filter(std::unordered_set<Invocation *> &source, std::unordered_set<Invocation *> &target) const {
            for (Invocation *i:source) {
                if (shouldInclude(i)) {
                    target.insert(i);
                }
            }
        }
    };
}
