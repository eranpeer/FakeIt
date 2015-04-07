//
// Created by eran on 01/04/2015.
//

#ifndef ACTUALINVOCATIONSSOURCE_HPP_
#define ACTUALINVOCATIONSSOURCE_HPP_

#include <unordered_set>
#include "fakeit/Invocation.hpp"

namespace fakeit {

	struct ActualInvocationsSource {
		virtual void getActualInvocations(std::unordered_set<fakeit::Invocation *> &into) const = 0;
		virtual ~ActualInvocationsSource() THROWS {};
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

		AggregateInvocationsSource(std::set<ActualInvocationsSource *> &sources) : _sources(sources) {
		}

		void getActualInvocations(std::unordered_set<fakeit::Invocation *> &into) const override {
			for (ActualInvocationsSource *source : _sources) {
				source->getActualInvocations(into);
			}
		}

	private:
		std::set<ActualInvocationsSource *> _sources;
	};
}
#endif //ACTUALINVOCATIONSSOURCE_HPP_
