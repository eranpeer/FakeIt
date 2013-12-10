#ifndef Table_h__
#define Table_h__

#include <array>
#include "utils.h"

template <int N>
struct Table {

	Table(){
	}

	~Table() {
	}

	void set(unsigned int index, std::shared_ptr<Destructable> value)
	{
		_values[index] = value;
	}

	template <typename T>
	T get(unsigned int offset)
	{
		T value = reinterpret_cast<T>(get(offset).get());
		return value;
	}

	unsigned int size(){
		return _values.size();
	}

private:
	std::array<std::shared_ptr<Destructable>, N> _values;

	std::shared_ptr<Destructable> get(unsigned int index){
		return _values[index];
	}
};
#endif // Table_h__