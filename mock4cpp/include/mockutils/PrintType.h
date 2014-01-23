#ifndef HH_PRINT_TYPE
#define HH_PRINT_TYPE

#include <ostream>

namespace mine {

	template<class T_> struct PrintType
	{
		// Never used directly.
	};

	    /**
		     * For plain types we don't know.
			      *
				       * Could be enhanced by using type_traits.
					        */
	template<class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_>)
	{
		os << "T";
		return os;
	};

	    /**
		     * For void.
			      */
	template<>
	std::ostream& operator<<(std::ostream& os, PrintType<void>)
	{
		os << "void";
		return os;
	};

	    /**
		     * For int.
			      */
	template<>
	std::ostream& operator<<(std::ostream& os, PrintType<int>)
	{
		os << "int";
		return os;
	};

	    /**
		     * For char.
			      */
	template<>
	std::ostream& operator<<(std::ostream& os, PrintType<char>)
	{
		os << "char";
		return os;
	};

	    /**
		     * For const qualified types.
			      */
	template<class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_ const>)
	{
		os << PrintType<T_>() << " const";
		return os;
	};

	    /**
		     * For volatile qualified types.
			      */
	template<class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_ volatile>)
	{
		os << PrintType<T_>() << " volatile";
		return os;
	};

	    /**
		     * For const volatile qualified types.
			      */
	template<class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_ const volatile>)
	{
		os << PrintType<T_>() << " const volatile";
		return os;
	};

	    /**
		     * For reference types.
			      */
	template<class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_&>)
	{
		os << PrintType<T_>() << "&";
		return os;
	};

	    /**
		     * For pointers.
			      */
	template<class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_*>)
	{
		os << PrintType<T_>() << "*";
		return os;
	}

	    /**
		     * For pointers to members.
			      */
	template<class ClassT_, class MemberT_>
	std::ostream& operator<<(std::ostream& os, PrintType<MemberT_ ClassT_::*>)
	{
		os << PrintType<MemberT_>() << " " << PrintType<ClassT_>() << "::*";
		return os;
	}

	    /**
		     * Helper function for array types.
			      */
	template<std::size_t SizeP_, class T_>
	std::string printDimensions(PrintType<T_[SizeP_]>)
	{
		return
			"[" + boost::lexical_cast<std::string>(SizeP_)+"]"
			+ printDimensions(PrintType<T_>());
	}

	    /**
		     * Ends the recursion started by printDimensions(PrintType<T_[SizeP_]>).
			      */
	template<class T_>
	std::string printDimensions(PrintType<T_>)
	{
		return "";
	}

	    /**
		     * For arrays.
			      */
	template<std::size_t SizeP_, class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<T_[SizeP_]>)
	{
		os << PrintType<typename std::tr1::remove_all_extents<T_>::type>()
			<< printDimensions(PrintType<T_[SizeP_]>());

		return os;
	}

	    /**
		     * For const arrays.
			      */
	template<std::size_t SizeP_, class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<const T_[SizeP_]>)
	{
		os << "const " << PrintType<T_[SizeP_]>();
		return os;
	}

	    /**
		     * For volatile arrays.
			      */
	template<std::size_t SizeP_, class T_>
	std::ostream& operator<<(std::ostream& os, PrintType<volatile T_[SizeP_]>)
	{
		os << "volatile " << PrintType<T_[SizeP_]>();
		return os;
	}

	    /**
		     * For const volatile arrays.
			      */
	template<std::size_t SizeP_, class T_>
	std::ostream& operator<<(
		std::ostream& os,
		PrintType<const volatile T_[SizeP_]>)
	{
		os << "const volatile " << PrintType<T_[SizeP_]>();
		return os;
	}
}
#endif