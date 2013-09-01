#ifndef Table_h__
#define Table_h__

struct Table {

	Table(unsigned int size){
		_values = new void*[size];
		this->_size = size;
		for (unsigned int i = 0; i < size; i++){
			_values[i] = 0;
		}
	}

	~Table() {
		delete [] _values;
	}

	void set(unsigned int index, void *value)
	{
		if (index >= _size){
			throw "error";
		}
		_values[index] = value;
	}

	template <typename T>
	T get(unsigned int offset)
	{
		T value = reinterpret_cast<T>(get(offset));
		return value;
	}

	unsigned int size(){
		return _size;
	}

private:
	void** _values;
	unsigned int _size;

	void* get(unsigned int index){
		if (index >= _size){
			throw "error";
		}
		return _values[index];
	}
};
#endif // Table_h__


