/*
 * TpUnitAdapter.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 20, 2014
 */
#ifndef TPUNITADAPTER_HPP_
#define TPUNITADAPTER_HPP_

#include "tpunit++.hpp"
#include "fakeit/EventHandler.hpp"
#include "fakeit/DefaultFakeit.hpp"

namespace fakeit {

/**
 * This is a very simple testing framework adapter.
 * tpunit will log the message of any std::exception.
 * Just throw an std::exception with the error message.
 */
class TpUnitAdapter: public EventHandler {
public:

	class AssertionException: public std::runtime_error {
	public:
		AssertionException(std::string msg)
				: runtime_error(msg) {
		}
	};

	virtual ~TpUnitAdapter() = default;

	virtual void handle(const UnexpectedMethodCallEvent& e) {
		auto& fakeit = DefaultFakeit::getInstance();
		throw AssertionException(fakeit.format(e));
	}

	virtual void handle(const SequenceVerificationEvent& e) {
		auto& fakeit = DefaultFakeit::getInstance();
		throw AssertionException(fakeit.format(e));
	}

	virtual void handle(const NoMoreInvocationsVerificationEvent& e) {
		auto& fakeit = DefaultFakeit::getInstance();
		throw AssertionException(fakeit.format(e));
	}
};

}

#endif /* TPUNITADAPTER_HPP_ */
