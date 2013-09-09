#ifndef VirtualOffestSelector_h__
#define VirtualOffestSelector_h__

template < template <unsigned int> class OFFSET_CONSUMER>
struct VirtualOffsetSelector {

	virtual void * offset0(){ return new OFFSET_CONSUMER<0>(); }
	virtual void * offset1(){ return new OFFSET_CONSUMER<1>(); }
	virtual void * offset2(){ return new OFFSET_CONSUMER<2>(); }
	virtual void * offset3(){ return new OFFSET_CONSUMER<3>(); }
	virtual void * offset4(){ return new OFFSET_CONSUMER<4>(); }
	virtual void * offset5(){ return new OFFSET_CONSUMER<5>(); }
	virtual void * offset6(){ return new OFFSET_CONSUMER<6>(); }
	virtual void * offset7(){ return new OFFSET_CONSUMER<7>(); }
	virtual void * offset8(){ return new OFFSET_CONSUMER<8>(); }
	virtual void * offset9(){ return new OFFSET_CONSUMER<9>(); }
	virtual void * offset10(){ return new OFFSET_CONSUMER<10>(); }
	virtual void * offset11(){ return new OFFSET_CONSUMER<11>(); }
	virtual void * offset12(){ return new OFFSET_CONSUMER<12>(); }
	virtual void * offset13(){ return new OFFSET_CONSUMER<13>(); }
	virtual void * offset14(){ return new OFFSET_CONSUMER<14>(); }
	virtual void * offset15(){ return new OFFSET_CONSUMER<15>(); }
	virtual void * offset16(){ return new OFFSET_CONSUMER<16>(); }
	virtual void * offset17(){ return new OFFSET_CONSUMER<17>(); }
	virtual void * offset18(){ return new OFFSET_CONSUMER<18>(); }
	virtual void * offset19(){ return new OFFSET_CONSUMER<19>(); }
	virtual void * offset20(){ return new OFFSET_CONSUMER<20>(); }

	template <typename M>
	void * create(const M vMethod){
		auto sMethod = reinterpret_cast<void * (VirtualOffsetSelector::*)()>(vMethod);
		auto selectMethod = std::bind(sMethod, this);
		return selectMethod();
	}

};
#endif // VirtualOffestSelector_h__
