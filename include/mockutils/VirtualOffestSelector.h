#ifndef VirtualOffestSelector_h__
#define VirtualOffestSelector_h__

namespace fakeit {

template < template <unsigned int> class OFFSET_CONSUMER>
struct VirtualOffsetSelector {

	virtual OFFSET_CONSUMER<0> * offset0(){ return new OFFSET_CONSUMER<0>(); }
	virtual OFFSET_CONSUMER<1> * offset1(){ return new OFFSET_CONSUMER<1>(); }
	virtual OFFSET_CONSUMER<2> * offset2(){ return new OFFSET_CONSUMER<2>(); }
	virtual OFFSET_CONSUMER<3> * offset3(){ return new OFFSET_CONSUMER<3>(); }
	virtual OFFSET_CONSUMER<4> * offset4(){ return new OFFSET_CONSUMER<4>(); }
	virtual OFFSET_CONSUMER<5> * offset5(){ return new OFFSET_CONSUMER<5>(); }
	virtual OFFSET_CONSUMER<6> * offset6(){ return new OFFSET_CONSUMER<6>(); }
	virtual OFFSET_CONSUMER<7> * offset7(){ return new OFFSET_CONSUMER<7>(); }
	virtual OFFSET_CONSUMER<8> * offset8(){ return new OFFSET_CONSUMER<8>(); }
	virtual OFFSET_CONSUMER<9> * offset9(){ return new OFFSET_CONSUMER<9>(); }
	virtual OFFSET_CONSUMER<10> * offset10(){ return new OFFSET_CONSUMER<10>(); }
	virtual OFFSET_CONSUMER<11> * offset11(){ return new OFFSET_CONSUMER<11>(); }
	virtual OFFSET_CONSUMER<12> * offset12(){ return new OFFSET_CONSUMER<12>(); }
	virtual OFFSET_CONSUMER<13> * offset13(){ return new OFFSET_CONSUMER<13>(); }
	virtual OFFSET_CONSUMER<14> * offset14(){ return new OFFSET_CONSUMER<14>(); }
	virtual OFFSET_CONSUMER<15> * offset15(){ return new OFFSET_CONSUMER<15>(); }
	virtual OFFSET_CONSUMER<16> * offset16(){ return new OFFSET_CONSUMER<16>(); }
	virtual OFFSET_CONSUMER<17> * offset17(){ return new OFFSET_CONSUMER<17>(); }
	virtual OFFSET_CONSUMER<18> * offset18(){ return new OFFSET_CONSUMER<18>(); }
	virtual OFFSET_CONSUMER<19> * offset19(){ return new OFFSET_CONSUMER<19>(); }
	virtual OFFSET_CONSUMER<20> * offset20(){ return new OFFSET_CONSUMER<20>(); }
	virtual OFFSET_CONSUMER<21> * offset21(){ return new OFFSET_CONSUMER<21>(); }
	virtual OFFSET_CONSUMER<22> * offset22(){ return new OFFSET_CONSUMER<22>(); }
	virtual OFFSET_CONSUMER<23> * offset23(){ return new OFFSET_CONSUMER<23>(); }
	virtual OFFSET_CONSUMER<24> * offset24(){ return new OFFSET_CONSUMER<24>(); }
	virtual OFFSET_CONSUMER<25> * offset25(){ return new OFFSET_CONSUMER<25>(); }
	virtual OFFSET_CONSUMER<26> * offset26(){ return new OFFSET_CONSUMER<26>(); }
	virtual OFFSET_CONSUMER<27> * offset27(){ return new OFFSET_CONSUMER<27>(); }
	virtual OFFSET_CONSUMER<28> * offset28(){ return new OFFSET_CONSUMER<28>(); }
	virtual OFFSET_CONSUMER<29> * offset29(){ return new OFFSET_CONSUMER<29>(); }

	template <typename M>
	void * create(const M vMethod){
		auto sMethod = reinterpret_cast<void * (VirtualOffsetSelector::*)()>(vMethod);
		auto selectMethod = std::bind(sMethod, this);
		return selectMethod();
	}

};

}
#endif // VirtualOffestSelector_h__
