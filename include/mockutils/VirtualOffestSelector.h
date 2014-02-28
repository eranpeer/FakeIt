#ifndef VirtualOffestSelector_h__
#define VirtualOffestSelector_h__
#include "mockutils/union_cast.h"

namespace fakeit {

template<template<unsigned int> class OFFSET_CONSUMER>
struct VirtualOffsetSelector {

	virtual OFFSET_CONSUMER<0> * offset0() {
		return new OFFSET_CONSUMER<0>();
	}
	virtual OFFSET_CONSUMER<1> * offset1() {
		return new OFFSET_CONSUMER<1>();
	}
	virtual OFFSET_CONSUMER<2> * offset2() {
		return new OFFSET_CONSUMER<2>();
	}
	virtual OFFSET_CONSUMER<3> * offset3() {
		return new OFFSET_CONSUMER<3>();
	}
	virtual OFFSET_CONSUMER<4> * offset4() {
		return new OFFSET_CONSUMER<4>();
	}
	virtual OFFSET_CONSUMER<5> * offset5() {
		return new OFFSET_CONSUMER<5>();
	}
	virtual OFFSET_CONSUMER<6> * offset6() {
		return new OFFSET_CONSUMER<6>();
	}
	virtual OFFSET_CONSUMER<7> * offset7() {
		return new OFFSET_CONSUMER<7>();
	}
	virtual OFFSET_CONSUMER<8> * offset8() {
		return new OFFSET_CONSUMER<8>();
	}
	virtual OFFSET_CONSUMER<9> * offset9() {
		return new OFFSET_CONSUMER<9>();
	}
	virtual OFFSET_CONSUMER<10> * offset10() {
		return new OFFSET_CONSUMER<10>();
	}
	virtual OFFSET_CONSUMER<11> * offset11() {
		return new OFFSET_CONSUMER<11>();
	}
	virtual OFFSET_CONSUMER<12> * offset12() {
		return new OFFSET_CONSUMER<12>();
	}
	virtual OFFSET_CONSUMER<13> * offset13() {
		return new OFFSET_CONSUMER<13>();
	}
	virtual OFFSET_CONSUMER<14> * offset14() {
		return new OFFSET_CONSUMER<14>();
	}
	virtual OFFSET_CONSUMER<15> * offset15() {
		return new OFFSET_CONSUMER<15>();
	}
	virtual OFFSET_CONSUMER<16> * offset16() {
		return new OFFSET_CONSUMER<16>();
	}
	virtual OFFSET_CONSUMER<17> * offset17() {
		return new OFFSET_CONSUMER<17>();
	}
	virtual OFFSET_CONSUMER<18> * offset18() {
		return new OFFSET_CONSUMER<18>();
	}
	virtual OFFSET_CONSUMER<19> * offset19() {
		return new OFFSET_CONSUMER<19>();
	}
	virtual OFFSET_CONSUMER<20> * offset20() {
		return new OFFSET_CONSUMER<20>();
	}
	virtual OFFSET_CONSUMER<21> * offset21() {
		return new OFFSET_CONSUMER<21>();
	}
	virtual OFFSET_CONSUMER<22> * offset22() {
		return new OFFSET_CONSUMER<22>();
	}
	virtual OFFSET_CONSUMER<23> * offset23() {
		return new OFFSET_CONSUMER<23>();
	}
	virtual OFFSET_CONSUMER<24> * offset24() {
		return new OFFSET_CONSUMER<24>();
	}
	virtual OFFSET_CONSUMER<25> * offset25() {
		return new OFFSET_CONSUMER<25>();
	}
	virtual OFFSET_CONSUMER<26> * offset26() {
		return new OFFSET_CONSUMER<26>();
	}
	virtual OFFSET_CONSUMER<27> * offset27() {
		return new OFFSET_CONSUMER<27>();
	}
	virtual OFFSET_CONSUMER<28> * offset28() {
		return new OFFSET_CONSUMER<28>();
	}
	virtual OFFSET_CONSUMER<29> * offset29() {
		return new OFFSET_CONSUMER<29>();
	}
	virtual OFFSET_CONSUMER<30> * offset30() {
		return new OFFSET_CONSUMER<30>();
	}
	virtual OFFSET_CONSUMER<31> * offset31() {
		return new OFFSET_CONSUMER<31>();
	}
	virtual OFFSET_CONSUMER<32> * offset32() {
		return new OFFSET_CONSUMER<32>();
	}
	virtual OFFSET_CONSUMER<33> * offset33() {
		return new OFFSET_CONSUMER<33>();
	}
	virtual OFFSET_CONSUMER<34> * offset34() {
		return new OFFSET_CONSUMER<34>();
	}
	virtual OFFSET_CONSUMER<35> * offset35() {
		return new OFFSET_CONSUMER<35>();
	}
	virtual OFFSET_CONSUMER<36> * offset36() {
		return new OFFSET_CONSUMER<36>();
	}
	virtual OFFSET_CONSUMER<37> * offset37() {
		return new OFFSET_CONSUMER<37>();
	}
	virtual OFFSET_CONSUMER<38> * offset38() {
		return new OFFSET_CONSUMER<38>();
	}
	virtual OFFSET_CONSUMER<39> * offset39() {
		return new OFFSET_CONSUMER<39>();
	}
	virtual OFFSET_CONSUMER<40> * offset40() {
		return new OFFSET_CONSUMER<40>();
	}
	virtual OFFSET_CONSUMER<41> * offset41() {
		return new OFFSET_CONSUMER<41>();
	}
	virtual OFFSET_CONSUMER<42> * offset42() {
		return new OFFSET_CONSUMER<42>();
	}
	virtual OFFSET_CONSUMER<43> * offset43() {
		return new OFFSET_CONSUMER<43>();
	}
	virtual OFFSET_CONSUMER<44> * offset44() {
		return new OFFSET_CONSUMER<44>();
	}
	virtual OFFSET_CONSUMER<45> * offset45() {
		return new OFFSET_CONSUMER<45>();
	}
	virtual OFFSET_CONSUMER<46> * offset46() {
		return new OFFSET_CONSUMER<46>();
	}
	virtual OFFSET_CONSUMER<47> * offset47() {
		return new OFFSET_CONSUMER<47>();
	}
	virtual OFFSET_CONSUMER<48> * offset48() {
		return new OFFSET_CONSUMER<48>();
	}
	virtual OFFSET_CONSUMER<49> * offset49() {
		return new OFFSET_CONSUMER<49>();
	}


	template<typename M>
	void * create(const M vMethod) {
		//auto sMethod = union_cast<void * (VirtualOffsetSelector::*)()>(vMethod);
		auto sMethod = reinterpret_cast<void * (VirtualOffsetSelector::*)()>(vMethod);
		auto selectMethod = std::bind(sMethod, this);
		return selectMethod();
	}

};

}
#endif // VirtualOffestSelector_h__
