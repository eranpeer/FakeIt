#ifndef VirtualMethodOffsetLocator_h__
#define VirtualMethodOffsetLocator_h__

struct VirtualMethodOffsetLocator {

	virtual unsigned int offset0(){ return 0; }
	virtual unsigned int offset1(){ return 1; }
	virtual unsigned int offset2(){ return 2; }
	virtual unsigned int offset3(){ return 3; }
	virtual unsigned int offset4(){ return 4; }
	virtual unsigned int offset5(){ return 5; }
	virtual unsigned int offset6(){ return 6; }
	virtual unsigned int offset7(){ return 7; }
	virtual unsigned int offset8(){ return 8; }
	virtual unsigned int offset9(){ return 9; }

	template <typename M>
	unsigned int getOffset(const M vMethod){
		auto sMethod = reinterpret_cast<int (VirtualMethodOffsetLocator::*)()>(vMethod);
		auto selectMethod = std::bind(sMethod, this);
		return selectMethod();
	}
};

#endif // VirtualMethodOffsetLocator_h__
