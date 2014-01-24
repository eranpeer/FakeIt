#ifndef HH_PRINT_TYPE
#define HH_PRINT_TYPE

#include <ostream>

template<class T_> struct PrintType {
	// Never used directly.
};
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_>) {
	os << typeid(T_).name();
	return os;
}
;
template<>
std::ostream& operator<<(std::ostream& os, PrintType<void>) {
	os << "void";
	return os;
}
;
template<>
std::ostream& operator<<(std::ostream& os, PrintType<int>) {
	os << "int";
	return os;
}
;
template<>
std::ostream& operator<<(std::ostream& os, PrintType<char>) {
	os << "char";
	return os;
}
;
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_ const>) {
	os << PrintType<T_>() << " const";
	return os;
}
;
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_ volatile>) {
	os << PrintType<T_>() << " volatile";
	return os;
}
;
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_ const volatile>) {
	os << PrintType<T_>() << " const volatile";
	return os;
}
;
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_&>) {
	os << PrintType<T_>() << "&";
	return os;
}
;
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_*>) {
	os << PrintType<T_>() << "*";
	return os;
}
template<class ClassT_, class MemberT_>
std::ostream& operator<<(std::ostream& os, PrintType<MemberT_ ClassT_::*>) {
	os << PrintType<MemberT_>() << " " << PrintType<ClassT_>() << "::*";
	return os;
}
template<std::size_t SizeP_, class T_>
std::string printDimensions(PrintType<T_[SizeP_]>) {
	return "[" +std::to_string(SizeP_) + "]" + printDimensions(PrintType<T_>());
}

template<class T_>
std::string printDimensions(PrintType<T_>) {
	return "";
}
template<std::size_t SizeP_, class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_[SizeP_]>) {
	os << PrintType<typename std::remove_all_extents<T_>::type>() << printDimensions(PrintType<T_[SizeP_]>());

	return os;
}
template<std::size_t SizeP_, class T_>
std::ostream& operator<<(std::ostream& os, PrintType<const T_[SizeP_]>) {
	os << "const " << PrintType<T_[SizeP_]>();
	return os;
}
template<std::size_t SizeP_, class T_>
std::ostream& operator<<(std::ostream& os, PrintType<volatile T_[SizeP_]>) {
	os << "volatile " << PrintType<T_[SizeP_]>();
	return os;
}
template<std::size_t SizeP_, class T_>
std::ostream& operator<<(std::ostream& os, PrintType<const volatile T_[SizeP_]>) {
	os << "const volatile " << PrintType<T_[SizeP_]>();
	return os;
}

/**
 * Helper struct for printing parameter lists.
 *
 * @tparam StartsP_
 *  true iff we are at the start of the list (needed to omit
 *  commas in the output).
 * @tparam ArgsT_
 *  the actual arguments.
 */
template<bool StartsP_, class ... ArgsT_> struct PrintParameters {
	// Never used directly.
};

/**
 * For parameter lists.
 */
template<bool StartsP_, class ArgT, class ... ArgsT_>
std::ostream& operator<<(std::ostream& os, PrintParameters<StartsP_, ArgT, ArgsT_...>) {
	os << (StartsP_ ? "" : ", ") << PrintType<ArgT>() << PrintParameters<false, ArgsT_...>();

	return os;
}

/**
 * Terminates the recursion.
 */
template<bool StartsP_>
std::ostream& operator<<(std::ostream& os, PrintParameters<StartsP_>) {
	return os;
}

/**
 * For function pointers.
 */
template<class RetT_, class ... ArgsT_>
std::ostream& operator<<(std::ostream& os, PrintType<RetT_(ArgsT_...)>) {
	os << PrintType<RetT_>() << "(" << PrintParameters<true, ArgsT_...>() << ")";

	return os;
}

/**
 * For member functions.
 */
template<class ClassT_, class RetT_, class ... ArgsT_>
std::ostream& operator<<(std::ostream& os, PrintType<RetT_ (ClassT_::*)(ArgsT_...)>) {
	os << PrintType<RetT_>() << "(" << PrintType<ClassT_>() << "::*)(" << PrintParameters<true, ArgsT_...>() << ")";

	return os;
}

/**
 * For parameter lists with varargs.
 */
template<bool StartsP_, class ... ArgsT_> struct PrintParamtersWithVarArgs {
	// Never used directly.
};

/**
 * For function pointers with varargs.
 */
template<class RetT_, class ... ArgsT_>
std::ostream& operator<<(std::ostream& os, PrintType<RetT_(ArgsT_... ...)>)
{
	os << PrintType<RetT_>()
	<< "(" << PrintParameters<true, ArgsT_...>() << "...)";

	return os;
}

/**
 * For member functions with varargs.
 */
template<class RetT_, class ClassT_, class ... ArgsT_>
std::ostream& operator<<(std::ostream& os, PrintType<RetT_ (ClassT_::*)(ArgsT_... ...)>)
{
	os << PrintType<RetT_>()
	<< "(" << PrintType<ClassT_>()
	<< "::*)(" << PrintParameters<true, ArgsT_...>() << "...)";

	return os;
}

/**
 * For rvalue references (for completeness).
 */
template<class T_>
std::ostream& operator<<(std::ostream& os, PrintType<T_&&>) {
	os << PrintType<T_>() << "&&";
	return os;
}


#endif
