#ifndef VirtualTable_h__
#define VirtualTable_h__

template <int SIZE>
struct VirtualTable {

	VirtualTable(){
		methods = new void*[SIZE];
		for (unsigned int i = 0; i < SIZE; i++){
			methods[i] = 0;
		}
	}

	~VirtualTable() {
		delete [] methods;
	}

	void setMethod(unsigned int index, void *method)
	{
		if (index >= SIZE){
			throw "error";
		}
		methods[index] = method;
	}

	void * getMethod(unsigned int index){
		return methods[index];
	}

	unsigned int getSize(){
		return SIZE;
	}

private:
	void** methods;
};
#endif // VirtualTable_h__