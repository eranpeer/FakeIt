/*
 * to_string.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Sep 10, 2014
 */
#ifndef TO_STRING_HPP_
#define TO_STRING_HPP_



#include <string>
#include <sstream>
#include <iomanip>

namespace fakeit {
    
	template < typename T > static std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }

	/*
	static std::string to_string(const double& n)
    {
        std::ostringstream stm ;
        stm <<std::setprecision(6) << std::setiosflags(std::ios::fixed) <<n ;
        std::string rv{stm.str()};
        return rv;
    }
	*/
	/*
	static std::string to_string(const long double& n)
	{
		std::ostringstream stm;
		stm << std::setprecision(6) << std::setiosflags(std::ios::fixed) << n;
		std::string rv{ stm.str() };
		return rv;
	}
	*/
}

#endif /* TO_STRING_HPP_ */