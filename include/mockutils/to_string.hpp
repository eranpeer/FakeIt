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

namespace fakeit{

	// Define to_string since MinGW32 does not support std::to_string
    template < typename T > static std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }

}

#endif /* TO_STRING_HPP_ */
