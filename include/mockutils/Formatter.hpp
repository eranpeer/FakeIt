#ifndef HH_PRINT_TYPE
#define HH_PRINT_TYPE

#include <ostream>
#include <type_traits>
#include <string>

namespace fakeit {

template<class T> struct Formatter {
	static std::string format(const T& val) {
		if (std::is_const < T > ::value)
			return Formatter<typename std::remove_const<T>::type>::format(val);
		if (std::is_reference < T > ::value)
			return Formatter<typename std::remove_reference<T>::type>::format(val);
		if (std::is_volatile < T > ::value)
			return Formatter<typename std::remove_volatile<T>::type>::format(val);

		return {"?"};
	}
};

template<> struct Formatter<int> {
	static std::string format(const int& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<unsigned int> {
	static std::string format(const unsigned int& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<long> {
	static std::string format(const long& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<unsigned long> {
	static std::string format(const unsigned long& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<long long> {
	static std::string format(const long long& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<unsigned long long> {
	static std::string format(const unsigned long long& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<long double> {
	static std::string format(const long double& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<double> {
	static std::string format(const double& val) {
		return std::to_string(val);
	}
};

template<> struct Formatter<float> {
	static std::string format(const float& val) {
		return std::to_string(val);
	}
};

}
#endif
