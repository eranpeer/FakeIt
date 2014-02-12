#ifndef HH_PRINT_TYPE
#define HH_PRINT_TYPE

#include <ostream>
#include <type_traits>
#include <string>

namespace fakeit {

//template<class T>
//std::ostream & operator<<(std::ostream &strm, const T& t) {
//	if (std::is_const < T > ::value)
//		strm << t;
////	if (std::is_reference < T > ::value)
////		return Formatter<typename std::remove_reference<T>::type>::format(val);
////	if (std::is_volatile < T > ::value)
////		return Formatter<typename std::remove_volatile<T>::type>::format(val);
////	return {"?"};
//
//	return strm;
//}


template<class T> struct Formatter {
	static std::string format(const T& val) {
		if (std::is_const < T > ::value)
			return Formatter<typename std::remove_const<T>::type>::format(val);
		if (std::is_reference < T > ::value)
			return Formatter<typename std::remove_reference<T>::type>::format(val);
		if (std::is_volatile < T > ::value)
			return Formatter<typename std::remove_volatile<T>::type>::format(val);
//		if (std::is_pointer < T > ::value)
//			return Formatter<typename std::remove_pointer<T>::type>::format(*val);

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

//
//template<>
//std::ostream& operator<<(std::ostream& os, Formatter<void>) {
//	os << "void";
//	return os;
//}
//
//template<>
//std::ostream& operator<<(std::ostream& os, Formatter<int>) {
//	os << "int";
//	return os;
//}
//
//template<>
//std::ostream& operator<<(std::ostream& os, Formatter<char>) {
//	os << "char";
//	return os;
//}
//
//template<class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_ const>) {
//	os << Formatter<T_>() << " const";
//	return os;
//}
//
//template<class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_ volatile>) {
//	os << Formatter<T_>() << " volatile";
//	return os;
//}
//
//template<class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_ const volatile>) {
//	os << Formatter<T_>() << " const volatile";
//	return os;
//}
//
//template<class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_&>) {
//	os << Formatter<T_>() << "&";
//	return os;
//}
//
//template<class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_*>) {
//	os << Formatter<T_>() << "*";
//	return os;
//}
//
//template<class ClassT_, class MemberT_>
//static std::ostream& operator<<(std::ostream& os, Formatter<MemberT_ ClassT_::*>) {
//	os << Formatter<MemberT_>() << " " << Formatter<ClassT_>() << "::*";
//	return os;
//}
//
//template<std::size_t SizeP_, class T_>
//static std::string printDimensions(Formatter<T_[SizeP_]>) {
//	return "[" +std::to_string(SizeP_) + "]" + printDimensions(Formatter<T_>());
//}
//
//template<class T_>
//static std::string printDimensions(Formatter<T_>) {
//	return "";
//}
//
//template<std::size_t SizeP_, class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_[SizeP_]>) {
//	os << Formatter<typename std::remove_all_extents<T_>::type>() << printDimensions(Formatter<T_[SizeP_]>());
//
//	return os;
//}
//
//template<std::size_t SizeP_, class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<const T_[SizeP_]>) {
//	os << "const " << Formatter<T_[SizeP_]>();
//	return os;
//}
//
//template<std::size_t SizeP_, class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<volatile T_[SizeP_]>) {
//	os << "volatile " << Formatter<T_[SizeP_]>();
//	return os;
//}
//
//template<std::size_t SizeP_, class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<const volatile T_[SizeP_]>) {
//	os << "const volatile " << Formatter<T_[SizeP_]>();
//	return os;
//}
//
///**
// * Helper struct for printing parameter lists.
// *
// * @tparam StartsP_
// *  true if we are at the start of the list (needed to omit
// *  commas in the output).
// * @tparam ArgsT_
// *  the actual arguments.
// */
//template<bool StartsP_, class ... ArgsT_> struct PrintParameters {
//	// Never used directly.
//};
//
///**
// * For parameter lists.
// */
//template<bool StartsP_, class ArgT, class ... ArgsT_>
//static std::ostream& operator<<(std::ostream& os, PrintParameters<StartsP_, ArgT, ArgsT_...>) {
//	os << (StartsP_ ? "" : ", ") << Formatter<ArgT>() << PrintParameters<false, ArgsT_...>();
//
//	return os;
//}
//
///**
// * Terminates the recursion.
// */
//template<bool StartsP_>
//static std::ostream& operator<<(std::ostream& os, PrintParameters<StartsP_>) {
//	return os;
//}
//
///**
// * For function pointers.
// */
//template<class RetT_, class ... ArgsT_>
//static std::ostream& operator<<(std::ostream& os, Formatter<RetT_(ArgsT_...)>) {
//	os << Formatter<RetT_>() << "(" << PrintParameters<true, ArgsT_...>() << ")";
//	return os;
//}
//
///**
// * For member functions.
// */
//template<class ClassT_, class RetT_, class ... ArgsT_>
//static std::ostream& operator<<(std::ostream& os, Formatter<RetT_(ClassT_::*)(ArgsT_...)>) {
//	os << Formatter<RetT_>() << "(" << Formatter<ClassT_>() << "::*)(" << PrintParameters<true, ArgsT_...>() << ")";
//
//	return os;
//}
//
///**
// * For parameter lists with varargs.
// */
//template<bool StartsP_, class ... ArgsT_> struct PrintParamtersWithVarArgs {
//	// Never used directly.
//};
//
///**
// * For function pointers with varargs.
// */
////template<class RetT_, class ... ArgsT_>
////std::ostream& operator<<(std::ostream& os, Formatter<RetT_(ArgsT_... ...)>)
////{
////	os << Formatter<RetT_>()
////	<< "(" << PrintParameters<true, ArgsT_...>() << "...)";
////
////	return os;
////}
//
///**
// * For member functions with varargs.
// */
////template<class RetT_, class ClassT_, class ... ArgsT_>
////std::ostream& operator<<(std::ostream& os, Formatter<RetT_ (ClassT_::*)(ArgsT_... ...)>)
////{
////	os << Formatter<RetT_>()
////	<< "(" << Formatter<ClassT_>()
////	<< "::*)(" << PrintParameters<true, ArgsT_...>() << "...)";
////	return os;
////}
//
///**
// * For rvalue references (for completeness).
// */
//template<class T_>
//static std::ostream& operator<<(std::ostream& os, Formatter<T_&&>) {
//	os << Formatter<T_>() << "&&";
//	return os;
//}
//
//

}
#endif
