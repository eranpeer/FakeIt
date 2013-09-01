#ifndef VirtualTable_h__
#define VirtualTable_h__

struct VirtualTable {

	VirtualTable(unsigned int size){
		methods = new void*[size];
		this->size = size;
		for (unsigned int i = 0; i < size; i++){
			methods[i] = 0;
		}
	}

	~VirtualTable(){
		delete [] methods;
	}

	void setMethod(unsigned int index, void *method)
	{
		if (index >= size){
			throw "error";
		}
		methods[index] = method;
	}

	void * getMethod(unsigned int index){
		return methods[index];
	}

	unsigned int getSize(){
		return size;
	}

private:
	void** methods;
	unsigned int size;
};
#endif // VirtualTable_h__


