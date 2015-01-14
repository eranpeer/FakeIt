/*
 * argument_matchers.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jan 12, 2015
 */
#ifndef ARGUMENT_MATCHERS_HPP_
#define ARGUMENT_MATCHERS_HPP_

namespace fakeit {

struct IMatcher: public Destructable  {
	virtual std::string format() = 0;
};

template<typename T>
struct ITypedMatcher: public IMatcher  {
	virtual bool matches(const T& actual) const = 0;
};

template<typename T>
struct ITypedArgumentMatcher{

	virtual ~ITypedArgumentMatcher() = default;

	virtual ITypedMatcher<T> * createMatcher() const =0;
};

template<typename T>
struct ArgumentMatcher: public ITypedArgumentMatcher<T> {

	virtual ~ArgumentMatcher() = default;

	ArgumentMatcher(const T& arg)
			: _expected(arg) {
	}

	struct Matcher: public ITypedMatcher<T> {
		Matcher(const T& expected)
				: _expected(expected) {
		}

		virtual bool matches(const T& actual) const {
			return actual == _expected;
		}

		const T _expected;
	};

	const T& _expected;
};


template <typename T>
struct TypedAnyMatcher: public ITypedArgumentMatcher<T> {

	virtual ~TypedAnyMatcher() = default;

	TypedAnyMatcher() {
	}

	struct Matcher: public ITypedMatcher<T> {
		virtual bool matches(const T& actual) const {
			return true;
		}

		virtual std::string format() override {
			return "Any";
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher();
	}

};


template<typename T>
struct EqualsMathcher: public ArgumentMatcher<T> {

	virtual ~EqualsMathcher() = default;

	EqualsMathcher(const T& expected)
			: ArgumentMatcher<T>(expected) {
	}

	struct Matcher: public ArgumentMatcher<T>::Matcher {
		Matcher(const T& expected)
				: ArgumentMatcher<T>::Matcher(expected) {
		}

		virtual std::string format() override {
			return Formatter<T>::format(this->_expected);
		}
	};


	virtual ITypedMatcher<T> * createMatcher() const {
		return new Matcher(this->_expected);
	}

};

template<typename T>
struct GreaterThanMatcher: public ArgumentMatcher<T> {

	virtual ~GreaterThanMatcher() = default;

	GreaterThanMatcher(const T& expected)
			: ArgumentMatcher<T>(expected) {
	}

	struct Matcher: public ArgumentMatcher<T>::Matcher {
		Matcher(const T& expected)
				: ArgumentMatcher<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const {
			return actual > this->_expected;
		}
		virtual std::string format() override {
			return std::string(">") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}
};

template<typename T>
struct GreaterOrEqualsMatcher: public ArgumentMatcher<T> {

	virtual ~GreaterOrEqualsMatcher() = default;

	GreaterOrEqualsMatcher(const T& expected)
			: ArgumentMatcher<T>(expected) {
	}

	struct Matcher: public ArgumentMatcher<T>::Matcher {
		Matcher(const T& expected)
				: ArgumentMatcher<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const {
			return actual >= this->_expected;
		}
		virtual std::string format() override {
			return std::string(">=") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}
};


template<typename T>
struct LessThanMatcher: public ArgumentMatcher<T> {

	virtual ~LessThanMatcher() = default;

	LessThanMatcher(const T& expected)
			: ArgumentMatcher<T>(expected) {
	}

	struct Matcher: public ArgumentMatcher<T>::Matcher {
		Matcher(const T& expected)
				: ArgumentMatcher<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const {
			return actual < this->_expected;
		}
		virtual std::string format() override {
			return std::string("<") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}

};


template<typename T>
struct LessOrEqualsMatcher: public ArgumentMatcher<T> {

	virtual ~LessOrEqualsMatcher() = default;

	LessOrEqualsMatcher(const T& expected)
			: ArgumentMatcher<T>(expected) {
	}

	struct Matcher: public ArgumentMatcher<T>::Matcher {
		Matcher(const T& expected)
				: ArgumentMatcher<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const {
			return actual <= this->_expected;
		}
		virtual std::string format() override {
			return std::string("<=") + Formatter<T>::format(this->_expected);
		}

	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}

};


template<typename T>
struct NotEqualsMatcher: public ArgumentMatcher<T> {

	virtual ~NotEqualsMatcher() = default;

	NotEqualsMatcher(const T& expected)
			: ArgumentMatcher<T>(expected) {
	}

	struct Matcher: public ArgumentMatcher<T>::Matcher {
		Matcher(const T& expected)
				: ArgumentMatcher<T>::Matcher(expected) {
		}

		virtual bool matches(const T& actual) const {
			return actual != this->_expected;
		}

		virtual std::string format() override {
			return std::string("!=") + Formatter<T>::format(this->_expected);
		}

	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}

};

struct AnyMatcher{} static Any, _;

template<typename T>
EqualsMathcher<T> Eq(const T& arg) {
	EqualsMathcher<T> rv(arg);
	return rv;
}

template<typename T>
GreaterThanMatcher<T> Gt(const T& arg) {
	GreaterThanMatcher<T> rv(arg);
	return rv;
}

template<typename T>
GreaterOrEqualsMatcher<T> Ge(const T& arg) {
	GreaterOrEqualsMatcher<T> rv(arg);
	return rv;
}

template<typename T>
LessThanMatcher<T> Lt(const T& arg) {
	LessThanMatcher<T> rv(arg);
	return rv;
}

template<typename T>
LessOrEqualsMatcher<T> Le(const T& arg) {
	LessOrEqualsMatcher<T> rv(arg);
	return rv;
}

template<typename T>
NotEqualsMatcher<T> Ne(const T& arg) {
	NotEqualsMatcher<T> rv(arg);
	return rv;
}

}

#endif /* ARGUMENT_MATCHERS_HPP_ */
