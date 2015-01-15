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

struct IMatcher: public Destructable {
	virtual std::string format() const = 0;
};

template<typename T>
struct ITypedMatcher: public IMatcher {
	virtual bool matches(const T& actual) const = 0;
};

template<typename T>
struct ITypedMatcherCreator {

	virtual ~ITypedMatcherCreator() = default;

	virtual ITypedMatcher<T> * createMatcher() const =0;
};

template<typename T>
struct ComparisonMatcherCreator: public ITypedMatcherCreator<T> {

	virtual ~ComparisonMatcherCreator() = default;

	ComparisonMatcherCreator(const T& arg)
			: _expected(arg) {
	}

	struct Matcher: public ITypedMatcher<T> {
		Matcher(const T& expected)
				: _expected(expected) {
		}

		const T _expected;
	};

	const T& _expected;
};

template<typename T>
struct TypedAnyMatcher: public ITypedMatcherCreator<T> {

	virtual ~TypedAnyMatcher() = default;

	TypedAnyMatcher() {
	}

	struct Matcher: public ITypedMatcher<T> {
		virtual bool matches(const T& actual) const {
			return true;
		}

		virtual std::string format() const override {
			return "Any";
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher();
	}

};

template<typename T>
struct EqMatcherCreator: public ComparisonMatcherCreator<T> {

	virtual ~EqMatcherCreator() = default;

	EqMatcherCreator(const T& expected)
			: ComparisonMatcherCreator<T>(expected) {
	}

	struct Matcher: public ComparisonMatcherCreator<T>::Matcher {
		Matcher(const T& expected)
				: ComparisonMatcherCreator<T>::Matcher(expected) {
		}

		virtual std::string format() const override {
			return Formatter<T>::format(this->_expected);
		}

		virtual bool matches(const T& actual) const override {
			return actual == this->_expected;
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const {
		return new Matcher(this->_expected);
	}

};

template<typename T>
struct GtMatcherCreator: public ComparisonMatcherCreator<T> {

	virtual ~GtMatcherCreator() = default;

	GtMatcherCreator(const T& expected)
			: ComparisonMatcherCreator<T>(expected) {
	}

	struct Matcher: public ComparisonMatcherCreator<T>::Matcher {
		Matcher(const T& expected)
				: ComparisonMatcherCreator<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const override {
			return actual > this->_expected;
		}
		virtual std::string format() const override {
			return std::string(">") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}
};

template<typename T>
struct GeMatcherCreator: public ComparisonMatcherCreator<T> {

	virtual ~GeMatcherCreator() = default;

	GeMatcherCreator(const T& expected)
			: ComparisonMatcherCreator<T>(expected) {
	}

	struct Matcher: public ComparisonMatcherCreator<T>::Matcher {
		Matcher(const T& expected)
				: ComparisonMatcherCreator<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const override {
			return actual >= this->_expected;
		}
		virtual std::string format() const override {
			return std::string(">=") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}
};

template<typename T>
struct LtMatcherCreator: public ComparisonMatcherCreator<T> {

	virtual ~LtMatcherCreator() = default;

	LtMatcherCreator(const T& expected)
			: ComparisonMatcherCreator<T>(expected) {
	}

	struct Matcher: public ComparisonMatcherCreator<T>::Matcher {
		Matcher(const T& expected)
				: ComparisonMatcherCreator<T>::Matcher(expected) {
		}
		virtual bool matches(const T& actual) const override {
			return actual < this->_expected;
		}
		virtual std::string format() const override {
			return std::string("<") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}

};

template<typename T>
struct LeMatcherCreator: public ComparisonMatcherCreator<T> {

	virtual ~LeMatcherCreator() = default;

	LeMatcherCreator(const T& expected)
			: ComparisonMatcherCreator<T>(expected) {
	}

	struct Matcher: public ComparisonMatcherCreator<T>::Matcher {
		Matcher(const T& expected)
				: ComparisonMatcherCreator<T>::Matcher(expected) {
		}

		virtual bool matches(const T& actual) const override {
			return actual <= this->_expected;
		}

		virtual std::string format() const override {
			return std::string("<=") + Formatter<T>::format(this->_expected);
		}
	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}

};

template<typename T>
struct NeMatcherCreator: public ComparisonMatcherCreator<T> {

	virtual ~NeMatcherCreator() = default;

	NeMatcherCreator(const T& expected)
			: ComparisonMatcherCreator<T>(expected) {
	}

	struct Matcher: public ComparisonMatcherCreator<T>::Matcher {
		Matcher(const T& expected)
				: ComparisonMatcherCreator<T>::Matcher(expected) {
		}

		virtual bool matches(const T& actual) const override {
			return actual != this->_expected;
		}

		virtual std::string format() const override {
			return std::string("!=") + Formatter<T>::format(this->_expected);
		}

	};

	virtual ITypedMatcher<T> * createMatcher() const override {
		return new Matcher(this->_expected);
	}

};

struct AnyMatcher {
}static _;

template<typename T>
TypedAnyMatcher<T> Any() {
	TypedAnyMatcher<T> rv;
	return rv;
}

template<typename T>
EqMatcherCreator<T> Eq(const T& arg) {
	EqMatcherCreator<T> rv(arg);
	return rv;
}

template<typename T>
GtMatcherCreator<T> Gt(const T& arg) {
	GtMatcherCreator<T> rv(arg);
	return rv;
}

template<typename T>
GeMatcherCreator<T> Ge(const T& arg) {
	GeMatcherCreator<T> rv(arg);
	return rv;
}

template<typename T>
LtMatcherCreator<T> Lt(const T& arg) {
	LtMatcherCreator<T> rv(arg);
	return rv;
}

template<typename T>
LeMatcherCreator<T> Le(const T& arg) {
	LeMatcherCreator<T> rv(arg);
	return rv;
}

template<typename T>
NeMatcherCreator<T> Ne(const T& arg) {
	NeMatcherCreator<T> rv(arg);
	return rv;
}

}

#endif /* ARGUMENT_MATCHERS_HPP_ */
