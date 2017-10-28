#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"
#include "mockutils//Macros.hpp"
#include "mockutils/to_string.hpp"

namespace fakeit {

class TpUnitAdapter: public EventHandler {
	EventFormatter& _formatter;

    std::string formatLineNumner(std::string file, int num){
#ifndef __GNUG__
        return file + std::string("(") + fakeit::to_string(num) + std::string(")");
#else
        return file + std::string(":") + fakeit::to_string(num);
#endif
    }

public:

	class AssertionException: public std::exception{
        std::string _msg;
	public:
		AssertionException(std::string msg)
				: _msg(msg) {
		}

        const char* what() const NO_THROWS override{
            return _msg.c_str();
        }

        virtual ~AssertionException() NO_THROWS {}
	};

	virtual ~TpUnitAdapter() = default;
	TpUnitAdapter(EventFormatter& formatter):_formatter(formatter){}

	virtual void handle(const UnexpectedMethodCallEvent& e) {
		throw AssertionException(_formatter.format(e));
	}

	virtual void handle(const SequenceVerificationEvent& e) {
        std::string format(formatLineNumner(e.file(), e.line()) + ": " + _formatter.format(e));
        throw AssertionException(format);
	}

	virtual void handle(const NoMoreInvocationsVerificationEvent& e) {
        std::string format(formatLineNumner(e.file(), e.line()) + ": " + _formatter.format(e));
        throw AssertionException(format);
    }
};

class TpUnitFakeit: public DefaultFakeit {

public:
	virtual ~TpUnitFakeit() = default;

	TpUnitFakeit()
			: _formatter(), _tpunitAdapter(*this) {
	}

	static TpUnitFakeit &getInstance() {
		static TpUnitFakeit instance;
		return instance;
	}

protected:

	fakeit::EventHandler &accessTestingFrameworkAdapter() override {
		return _tpunitAdapter;
	}

	EventFormatter &accessEventFormatter() override {
		return _formatter;
	}

private:

	DefaultEventFormatter _formatter;
	TpUnitAdapter _tpunitAdapter;

};

}
