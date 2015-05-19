#pragma once

#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"
#include "catch.hpp"
namespace fakeit {

    struct VerificationException : public FakeitException {
        virtual ~VerificationException() = default;

        void setFileInfo(std::string file, int line, std::string callingMethod) {
            _file = file;
            _callingMethod = callingMethod;
            _line = line;
        }

        const std::string& file() const {
            return _file;
        }
        int line() const {
            return _line;
        }
        const std::string& callingMethod() const {
            return _callingMethod;
        }

    private:
        std::string _file;
        int _line;
        std::string _callingMethod;
    };

    struct NoMoreInvocationsVerificationException : public VerificationException {

        NoMoreInvocationsVerificationException(std::string format) : //
            _format(format) { //
        }

        virtual std::string what() const override {
            return _format;
        }
    private:
        std::string _format;
    };

    struct SequenceVerificationException : public VerificationException {
        SequenceVerificationException(const std::string& format) : //
            _format(format) //
        {
        }

        virtual std::string what() const override {
            return _format;
        }

    private:
        std::string _format;
    };

class CatchAdapter: public EventHandler {
    EventFormatter& _formatter;

    std::string formatLineNumner(std::string file, int num){
#ifndef __GNUG__
        return file + std::string("(") + std::to_string(num) + std::string(")");
#else
        return file + std::string(":")+ std::to_string(num);
#endif
    }

public:

	virtual ~CatchAdapter() = default;
    CatchAdapter(EventFormatter& formatter)
        : _formatter(formatter){}

    virtual void handle(const UnexpectedMethodCallEvent &evt) override {
        std::string format = _formatter.format(evt);
        UnexpectedMethodCallException ex(format);
        throw ex;
    }

    virtual void handle(const SequenceVerificationEvent &evt) override {
        std::string format(formatLineNumner(evt.file(),evt.line()) + ": " + _formatter.format(evt));
        SequenceVerificationException e(format);
        e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
        throw e;
    }

    virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override {
        std::string format(formatLineNumner(evt.file(), evt.line()) + ": " + _formatter.format(evt));
        NoMoreInvocationsVerificationException e(format);
        e.setFileInfo(evt.file(), evt.line(), evt.callingMethod());
        throw e;
    }

};


class CatchFakeit: public DefaultFakeit {

	
public:

	virtual ~CatchFakeit() = default;

	template<typename T>
	class ExceptionTranslator : public Catch::IExceptionTranslator {
	public:
	ExceptionTranslator(){}
		virtual std::string translate() const {
			try {
				throw;
			}
			catch( T& ex ) {
				return ex.what();
			}
		}
	};

	CatchFakeit()
        : _formatter(), _catchAdapter(_formatter) {
			Catch::getMutableRegistryHub().registerTranslator
                (new ExceptionTranslator<UnexpectedMethodCallException>());
			Catch::getMutableRegistryHub().registerTranslator
                (new ExceptionTranslator<SequenceVerificationException>());
			Catch::getMutableRegistryHub().registerTranslator
                (new ExceptionTranslator<NoMoreInvocationsVerificationException>());
	}

	static CatchFakeit &getInstance() {
		static CatchFakeit instance;
		return instance;
	}

protected:

	fakeit::EventHandler &accessTestingFrameworkAdapter() override {
		return _catchAdapter;
	}

	EventFormatter &accessEventFormatter() override {
		return _formatter;
	}

private:

	DefaultEventFormatter _formatter;
	CatchAdapter _catchAdapter;
};

}
