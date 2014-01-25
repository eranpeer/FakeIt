#ifndef Formatter_h__
#define Formatter_h__

#include <iostream>
#include <string>
#include <sstream>

template<class T>
class Formatterr {
public:
	static std::string format( T& t) {
		std::stringstream sstream;
		sstream << t;
		std::string s = sstream.str();
		return s;
	}
};

#endif // Formatter_h__
