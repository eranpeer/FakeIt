/*
 * Accessor.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Aug 25, 2014
 */
#ifndef ACCESSOR_HPP_
#define ACCESSOR_HPP_

namespace fakeit {

template<typename O, typename T>
class Accessor {
public:
	Accessor(O* obj, T (O::*getter)()) :
			_obj(obj), _getter(getter) {
	}
	operator T() {
		return (_obj->*_getter)();
	}
private:
	O* _obj;
	T (O::*_getter)();
};

}

//	struct test
//	{
//
//		int real_i;
//		int get_i()
//		{
//			std::cout << "geting i" << std::endl;
//			return real_i;
//		}
//		Accessor<test,int> i;
//		test() :i(this,&test::get_i), real_i(0){};
//	};
//
//	void foo(){
//		test a;
//		std::cout << a.i << "\n" << std::endl;
//	}
//

#endif /* ACCESSOR_HPP_ */
