/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "mockutils/mscpp/FunctionWithConvention.hpp"

namespace fakeit {

    /// This class contains virtual methods that return their position in the virtual method table.
    /// Casting a pointer to the n:th virtual method of a class to a pointer to a method of this class
    /// will return a pointer to the n:th virtual method of this class, which when called will return n.
    template<typename CONVENTION>
	struct VirtualOffsetSelector{};

// Creates a virtual offset selector specialization for a specific calling convention.
#define FAKEIT_MAKE_SELECTOR( CONVENTION_TAG, CONVENTION_SPECIFIER ) \
	template<> \
    struct VirtualOffsetSelector<CONVENTION_TAG> { \
 \
        unsigned int offset; \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset0(int) { \
            return offset = 0; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset1(int) { \
            return offset = 1; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset2(int) { \
            return offset = 2; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset3(int) { \
            return offset = 3; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset4(int) { \
            return offset = 4; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset5(int) { \
            return offset = 5; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset6(int) { \
            return offset = 6; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset7(int) { \
            return offset = 7; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset8(int) { \
            return offset = 8; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset9(int) { \
            return offset = 9; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset10(int) { \
            return offset = 10; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset11(int) { \
            return offset = 11; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset12(int) { \
            return offset = 12; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset13(int) { \
            return offset = 13; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset14(int) { \
            return offset = 14; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset15(int) { \
            return offset = 15; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset16(int) { \
            return offset = 16; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset17(int) { \
            return offset = 17; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset18(int) { \
            return offset = 18; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset19(int) { \
            return offset = 19; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset20(int) { \
            return offset = 20; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset21(int) { \
            return offset = 21; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset22(int) { \
            return offset = 22; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset23(int) { \
            return offset = 23; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset24(int) { \
            return offset = 24; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset25(int) { \
            return offset = 25; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset26(int) { \
            return offset = 26; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset27(int) { \
            return offset = 27; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset28(int) { \
            return offset = 28; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset29(int) { \
            return offset = 29; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset30(int) { \
            return offset = 30; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset31(int) { \
            return offset = 31; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset32(int) { \
            return offset = 32; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset33(int) { \
            return offset = 33; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset34(int) { \
            return offset = 34; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset35(int) { \
            return offset = 35; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset36(int) { \
            return offset = 36; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset37(int) { \
            return offset = 37; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset38(int) { \
            return offset = 38; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset39(int) { \
            return offset = 39; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset40(int) { \
            return offset = 40; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset41(int) { \
            return offset = 41; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset42(int) { \
            return offset = 42; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset43(int) { \
            return offset = 43; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset44(int) { \
            return offset = 44; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset45(int) { \
            return offset = 45; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset46(int) { \
            return offset = 46; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset47(int) { \
            return offset = 47; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset48(int) { \
            return offset = 48; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset49(int) { \
            return offset = 49; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset50(int) { \
            return offset = 50; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset51(int) { \
            return offset = 51; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset52(int) { \
            return offset = 52; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset53(int) { \
            return offset = 53; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset54(int) { \
            return offset = 54; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset55(int) { \
            return offset = 55; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset56(int) { \
            return offset = 56; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset57(int) { \
            return offset = 57; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset58(int) { \
            return offset = 58; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset59(int) { \
            return offset = 59; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset60(int) { \
            return offset = 60; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset61(int) { \
            return offset = 61; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset62(int) { \
            return offset = 62; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset63(int) { \
            return offset = 63; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset64(int) { \
            return offset = 64; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset65(int) { \
            return offset = 65; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset66(int) { \
            return offset = 66; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset67(int) { \
            return offset = 67; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset68(int) { \
            return offset = 68; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset69(int) { \
            return offset = 69; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset70(int) { \
            return offset = 70; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset71(int) { \
            return offset = 71; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset72(int) { \
            return offset = 72; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset73(int) { \
            return offset = 73; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset74(int) { \
            return offset = 74; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset75(int) { \
            return offset = 75; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset76(int) { \
            return offset = 76; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset77(int) { \
            return offset = 77; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset78(int) { \
            return offset = 78; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset79(int) { \
            return offset = 79; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset80(int) { \
            return offset = 80; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset81(int) { \
            return offset = 81; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset82(int) { \
            return offset = 82; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset83(int) { \
            return offset = 83; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset84(int) { \
            return offset = 84; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset85(int) { \
            return offset = 85; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset86(int) { \
            return offset = 86; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset87(int) { \
            return offset = 87; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset88(int) { \
            return offset = 88; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset89(int) { \
            return offset = 89; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset90(int) { \
            return offset = 90; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset91(int) { \
            return offset = 91; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset92(int) { \
            return offset = 92; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset93(int) { \
            return offset = 93; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset94(int) { \
            return offset = 94; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset95(int) { \
            return offset = 95; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset96(int) { \
            return offset = 96; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset97(int) { \
            return offset = 97; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset98(int) { \
            return offset = 98; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset99(int) { \
            return offset = 99; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset100(int) { \
            return offset = 100; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset101(int) { \
            return offset = 101; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset102(int) { \
            return offset = 102; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset103(int) { \
            return offset = 103; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset104(int) { \
            return offset = 104; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset105(int) { \
            return offset = 105; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset106(int) { \
            return offset = 106; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset107(int) { \
            return offset = 107; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset108(int) { \
            return offset = 108; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset109(int) { \
            return offset = 109; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset110(int) { \
            return offset = 110; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset111(int) { \
            return offset = 111; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset112(int) { \
            return offset = 112; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset113(int) { \
            return offset = 113; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset114(int) { \
            return offset = 114; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset115(int) { \
            return offset = 115; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset116(int) { \
            return offset = 116; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset117(int) { \
            return offset = 117; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset118(int) { \
            return offset = 118; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset119(int) { \
            return offset = 119; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset120(int) { \
            return offset = 120; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset121(int) { \
            return offset = 121; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset122(int) { \
            return offset = 122; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset123(int) { \
            return offset = 123; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset124(int) { \
            return offset = 124; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset125(int) { \
            return offset = 125; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset126(int) { \
            return offset = 126; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset127(int) { \
            return offset = 127; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset128(int) { \
            return offset = 128; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset129(int) { \
            return offset = 129; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset130(int) { \
            return offset = 130; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset131(int) { \
            return offset = 131; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset132(int) { \
            return offset = 132; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset133(int) { \
            return offset = 133; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset134(int) { \
            return offset = 134; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset135(int) { \
            return offset = 135; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset136(int) { \
            return offset = 136; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset137(int) { \
            return offset = 137; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset138(int) { \
            return offset = 138; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset139(int) { \
            return offset = 139; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset140(int) { \
            return offset = 140; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset141(int) { \
            return offset = 141; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset142(int) { \
            return offset = 142; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset143(int) { \
            return offset = 143; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset144(int) { \
            return offset = 144; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset145(int) { \
            return offset = 145; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset146(int) { \
            return offset = 146; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset147(int) { \
            return offset = 147; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset148(int) { \
            return offset = 148; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset149(int) { \
            return offset = 149; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset150(int) { \
            return offset = 150; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset151(int) { \
            return offset = 151; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset152(int) { \
            return offset = 152; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset153(int) { \
            return offset = 153; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset154(int) { \
            return offset = 154; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset155(int) { \
            return offset = 155; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset156(int) { \
            return offset = 156; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset157(int) { \
            return offset = 157; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset158(int) { \
            return offset = 158; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset159(int) { \
            return offset = 159; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset160(int) { \
            return offset = 160; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset161(int) { \
            return offset = 161; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset162(int) { \
            return offset = 162; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset163(int) { \
            return offset = 163; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset164(int) { \
            return offset = 164; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset165(int) { \
            return offset = 165; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset166(int) { \
            return offset = 166; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset167(int) { \
            return offset = 167; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset168(int) { \
            return offset = 168; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset169(int) { \
            return offset = 169; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset170(int) { \
            return offset = 170; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset171(int) { \
            return offset = 171; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset172(int) { \
            return offset = 172; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset173(int) { \
            return offset = 173; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset174(int) { \
            return offset = 174; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset175(int) { \
            return offset = 175; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset176(int) { \
            return offset = 176; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset177(int) { \
            return offset = 177; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset178(int) { \
            return offset = 178; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset179(int) { \
            return offset = 179; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset180(int) { \
            return offset = 180; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset181(int) { \
            return offset = 181; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset182(int) { \
            return offset = 182; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset183(int) { \
            return offset = 183; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset184(int) { \
            return offset = 184; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset185(int) { \
            return offset = 185; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset186(int) { \
            return offset = 186; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset187(int) { \
            return offset = 187; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset188(int) { \
            return offset = 188; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset189(int) { \
            return offset = 189; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset190(int) { \
            return offset = 190; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset191(int) { \
            return offset = 191; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset192(int) { \
            return offset = 192; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset193(int) { \
            return offset = 193; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset194(int) { \
            return offset = 194; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset195(int) { \
            return offset = 195; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset196(int) { \
            return offset = 196; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset197(int) { \
            return offset = 197; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset198(int) { \
            return offset = 198; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset199(int) { \
            return offset = 199; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset200(int) { \
            return offset = 200; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset201(int) { \
            return offset = 201; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset202(int) { \
            return offset = 202; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset203(int) { \
            return offset = 203; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset204(int) { \
            return offset = 204; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset205(int) { \
            return offset = 205; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset206(int) { \
            return offset = 206; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset207(int) { \
            return offset = 207; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset208(int) { \
            return offset = 208; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset209(int) { \
            return offset = 209; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset210(int) { \
            return offset = 210; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset211(int) { \
            return offset = 211; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset212(int) { \
            return offset = 212; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset213(int) { \
            return offset = 213; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset214(int) { \
            return offset = 214; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset215(int) { \
            return offset = 215; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset216(int) { \
            return offset = 216; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset217(int) { \
            return offset = 217; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset218(int) { \
            return offset = 218; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset219(int) { \
            return offset = 219; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset220(int) { \
            return offset = 220; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset221(int) { \
            return offset = 221; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset222(int) { \
            return offset = 222; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset223(int) { \
            return offset = 223; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset224(int) { \
            return offset = 224; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset225(int) { \
            return offset = 225; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset226(int) { \
            return offset = 226; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset227(int) { \
            return offset = 227; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset228(int) { \
            return offset = 228; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset229(int) { \
            return offset = 229; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset230(int) { \
            return offset = 230; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset231(int) { \
            return offset = 231; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset232(int) { \
            return offset = 232; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset233(int) { \
            return offset = 233; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset234(int) { \
            return offset = 234; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset235(int) { \
            return offset = 235; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset236(int) { \
            return offset = 236; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset237(int) { \
            return offset = 237; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset238(int) { \
            return offset = 238; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset239(int) { \
            return offset = 239; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset240(int) { \
            return offset = 240; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset241(int) { \
            return offset = 241; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset242(int) { \
            return offset = 242; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset243(int) { \
            return offset = 243; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset244(int) { \
            return offset = 244; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset245(int) { \
            return offset = 245; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset246(int) { \
            return offset = 246; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset247(int) { \
            return offset = 247; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset248(int) { \
            return offset = 248; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset249(int) { \
            return offset = 249; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset250(int) { \
            return offset = 250; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset251(int) { \
            return offset = 251; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset252(int) { \
            return offset = 252; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset253(int) { \
            return offset = 253; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset254(int) { \
            return offset = 254; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset255(int) { \
            return offset = 255; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset256(int) { \
            return offset = 256; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset257(int) { \
            return offset = 257; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset258(int) { \
            return offset = 258; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset259(int) { \
            return offset = 259; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset260(int) { \
            return offset = 260; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset261(int) { \
            return offset = 261; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset262(int) { \
            return offset = 262; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset263(int) { \
            return offset = 263; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset264(int) { \
            return offset = 264; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset265(int) { \
            return offset = 265; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset266(int) { \
            return offset = 266; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset267(int) { \
            return offset = 267; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset268(int) { \
            return offset = 268; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset269(int) { \
            return offset = 269; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset270(int) { \
            return offset = 270; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset271(int) { \
            return offset = 271; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset272(int) { \
            return offset = 272; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset273(int) { \
            return offset = 273; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset274(int) { \
            return offset = 274; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset275(int) { \
            return offset = 275; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset276(int) { \
            return offset = 276; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset277(int) { \
            return offset = 277; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset278(int) { \
            return offset = 278; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset279(int) { \
            return offset = 279; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset280(int) { \
            return offset = 280; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset281(int) { \
            return offset = 281; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset282(int) { \
            return offset = 282; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset283(int) { \
            return offset = 283; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset284(int) { \
            return offset = 284; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset285(int) { \
            return offset = 285; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset286(int) { \
            return offset = 286; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset287(int) { \
            return offset = 287; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset288(int) { \
            return offset = 288; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset289(int) { \
            return offset = 289; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset290(int) { \
            return offset = 290; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset291(int) { \
            return offset = 291; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset292(int) { \
            return offset = 292; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset293(int) { \
            return offset = 293; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset294(int) { \
            return offset = 294; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset295(int) { \
            return offset = 295; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset296(int) { \
            return offset = 296; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset297(int) { \
            return offset = 297; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset298(int) { \
            return offset = 298; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset299(int) { \
            return offset = 299; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset300(int) { \
            return offset = 300; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset301(int) { \
            return offset = 301; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset302(int) { \
            return offset = 302; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset303(int) { \
            return offset = 303; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset304(int) { \
            return offset = 304; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset305(int) { \
            return offset = 305; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset306(int) { \
            return offset = 306; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset307(int) { \
            return offset = 307; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset308(int) { \
            return offset = 308; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset309(int) { \
            return offset = 309; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset310(int) { \
            return offset = 310; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset311(int) { \
            return offset = 311; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset312(int) { \
            return offset = 312; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset313(int) { \
            return offset = 313; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset314(int) { \
            return offset = 314; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset315(int) { \
            return offset = 315; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset316(int) { \
            return offset = 316; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset317(int) { \
            return offset = 317; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset318(int) { \
            return offset = 318; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset319(int) { \
            return offset = 319; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset320(int) { \
            return offset = 320; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset321(int) { \
            return offset = 321; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset322(int) { \
            return offset = 322; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset323(int) { \
            return offset = 323; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset324(int) { \
            return offset = 324; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset325(int) { \
            return offset = 325; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset326(int) { \
            return offset = 326; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset327(int) { \
            return offset = 327; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset328(int) { \
            return offset = 328; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset329(int) { \
            return offset = 329; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset330(int) { \
            return offset = 330; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset331(int) { \
            return offset = 331; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset332(int) { \
            return offset = 332; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset333(int) { \
            return offset = 333; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset334(int) { \
            return offset = 334; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset335(int) { \
            return offset = 335; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset336(int) { \
            return offset = 336; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset337(int) { \
            return offset = 337; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset338(int) { \
            return offset = 338; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset339(int) { \
            return offset = 339; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset340(int) { \
            return offset = 340; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset341(int) { \
            return offset = 341; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset342(int) { \
            return offset = 342; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset343(int) { \
            return offset = 343; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset344(int) { \
            return offset = 344; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset345(int) { \
            return offset = 345; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset346(int) { \
            return offset = 346; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset347(int) { \
            return offset = 347; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset348(int) { \
            return offset = 348; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset349(int) { \
            return offset = 349; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset350(int) { \
            return offset = 350; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset351(int) { \
            return offset = 351; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset352(int) { \
            return offset = 352; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset353(int) { \
            return offset = 353; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset354(int) { \
            return offset = 354; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset355(int) { \
            return offset = 355; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset356(int) { \
            return offset = 356; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset357(int) { \
            return offset = 357; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset358(int) { \
            return offset = 358; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset359(int) { \
            return offset = 359; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset360(int) { \
            return offset = 360; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset361(int) { \
            return offset = 361; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset362(int) { \
            return offset = 362; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset363(int) { \
            return offset = 363; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset364(int) { \
            return offset = 364; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset365(int) { \
            return offset = 365; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset366(int) { \
            return offset = 366; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset367(int) { \
            return offset = 367; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset368(int) { \
            return offset = 368; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset369(int) { \
            return offset = 369; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset370(int) { \
            return offset = 370; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset371(int) { \
            return offset = 371; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset372(int) { \
            return offset = 372; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset373(int) { \
            return offset = 373; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset374(int) { \
            return offset = 374; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset375(int) { \
            return offset = 375; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset376(int) { \
            return offset = 376; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset377(int) { \
            return offset = 377; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset378(int) { \
            return offset = 378; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset379(int) { \
            return offset = 379; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset380(int) { \
            return offset = 380; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset381(int) { \
            return offset = 381; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset382(int) { \
            return offset = 382; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset383(int) { \
            return offset = 383; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset384(int) { \
            return offset = 384; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset385(int) { \
            return offset = 385; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset386(int) { \
            return offset = 386; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset387(int) { \
            return offset = 387; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset388(int) { \
            return offset = 388; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset389(int) { \
            return offset = 389; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset390(int) { \
            return offset = 390; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset391(int) { \
            return offset = 391; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset392(int) { \
            return offset = 392; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset393(int) { \
            return offset = 393; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset394(int) { \
            return offset = 394; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset395(int) { \
            return offset = 395; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset396(int) { \
            return offset = 396; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset397(int) { \
            return offset = 397; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset398(int) { \
            return offset = 398; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset399(int) { \
            return offset = 399; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset400(int) { \
            return offset = 400; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset401(int) { \
            return offset = 401; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset402(int) { \
            return offset = 402; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset403(int) { \
            return offset = 403; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset404(int) { \
            return offset = 404; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset405(int) { \
            return offset = 405; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset406(int) { \
            return offset = 406; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset407(int) { \
            return offset = 407; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset408(int) { \
            return offset = 408; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset409(int) { \
            return offset = 409; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset410(int) { \
            return offset = 410; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset411(int) { \
            return offset = 411; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset412(int) { \
            return offset = 412; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset413(int) { \
            return offset = 413; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset414(int) { \
            return offset = 414; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset415(int) { \
            return offset = 415; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset416(int) { \
            return offset = 416; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset417(int) { \
            return offset = 417; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset418(int) { \
            return offset = 418; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset419(int) { \
            return offset = 419; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset420(int) { \
            return offset = 420; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset421(int) { \
            return offset = 421; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset422(int) { \
            return offset = 422; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset423(int) { \
            return offset = 423; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset424(int) { \
            return offset = 424; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset425(int) { \
            return offset = 425; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset426(int) { \
            return offset = 426; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset427(int) { \
            return offset = 427; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset428(int) { \
            return offset = 428; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset429(int) { \
            return offset = 429; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset430(int) { \
            return offset = 430; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset431(int) { \
            return offset = 431; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset432(int) { \
            return offset = 432; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset433(int) { \
            return offset = 433; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset434(int) { \
            return offset = 434; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset435(int) { \
            return offset = 435; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset436(int) { \
            return offset = 436; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset437(int) { \
            return offset = 437; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset438(int) { \
            return offset = 438; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset439(int) { \
            return offset = 439; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset440(int) { \
            return offset = 440; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset441(int) { \
            return offset = 441; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset442(int) { \
            return offset = 442; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset443(int) { \
            return offset = 443; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset444(int) { \
            return offset = 444; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset445(int) { \
            return offset = 445; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset446(int) { \
            return offset = 446; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset447(int) { \
            return offset = 447; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset448(int) { \
            return offset = 448; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset449(int) { \
            return offset = 449; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset450(int) { \
            return offset = 450; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset451(int) { \
            return offset = 451; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset452(int) { \
            return offset = 452; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset453(int) { \
            return offset = 453; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset454(int) { \
            return offset = 454; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset455(int) { \
            return offset = 455; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset456(int) { \
            return offset = 456; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset457(int) { \
            return offset = 457; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset458(int) { \
            return offset = 458; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset459(int) { \
            return offset = 459; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset460(int) { \
            return offset = 460; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset461(int) { \
            return offset = 461; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset462(int) { \
            return offset = 462; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset463(int) { \
            return offset = 463; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset464(int) { \
            return offset = 464; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset465(int) { \
            return offset = 465; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset466(int) { \
            return offset = 466; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset467(int) { \
            return offset = 467; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset468(int) { \
            return offset = 468; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset469(int) { \
            return offset = 469; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset470(int) { \
            return offset = 470; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset471(int) { \
            return offset = 471; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset472(int) { \
            return offset = 472; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset473(int) { \
            return offset = 473; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset474(int) { \
            return offset = 474; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset475(int) { \
            return offset = 475; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset476(int) { \
            return offset = 476; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset477(int) { \
            return offset = 477; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset478(int) { \
            return offset = 478; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset479(int) { \
            return offset = 479; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset480(int) { \
            return offset = 480; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset481(int) { \
            return offset = 481; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset482(int) { \
            return offset = 482; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset483(int) { \
            return offset = 483; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset484(int) { \
            return offset = 484; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset485(int) { \
            return offset = 485; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset486(int) { \
            return offset = 486; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset487(int) { \
            return offset = 487; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset488(int) { \
            return offset = 488; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset489(int) { \
            return offset = 489; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset490(int) { \
            return offset = 490; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset491(int) { \
            return offset = 491; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset492(int) { \
            return offset = 492; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset493(int) { \
            return offset = 493; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset494(int) { \
            return offset = 494; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset495(int) { \
            return offset = 495; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset496(int) { \
            return offset = 496; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset497(int) { \
            return offset = 497; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset498(int) { \
            return offset = 498; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset499(int) { \
            return offset = 499; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset500(int) { \
            return offset = 500; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset501(int) { \
            return offset = 501; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset502(int) { \
            return offset = 502; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset503(int) { \
            return offset = 503; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset504(int) { \
            return offset = 504; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset505(int) { \
            return offset = 505; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset506(int) { \
            return offset = 506; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset507(int) { \
            return offset = 507; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset508(int) { \
            return offset = 508; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset509(int) { \
            return offset = 509; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset510(int) { \
            return offset = 510; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset511(int) { \
            return offset = 511; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset512(int) { \
            return offset = 512; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset513(int) { \
            return offset = 513; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset514(int) { \
            return offset = 514; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset515(int) { \
            return offset = 515; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset516(int) { \
            return offset = 516; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset517(int) { \
            return offset = 517; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset518(int) { \
            return offset = 518; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset519(int) { \
            return offset = 519; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset520(int) { \
            return offset = 520; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset521(int) { \
            return offset = 521; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset522(int) { \
            return offset = 522; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset523(int) { \
            return offset = 523; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset524(int) { \
            return offset = 524; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset525(int) { \
            return offset = 525; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset526(int) { \
            return offset = 526; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset527(int) { \
            return offset = 527; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset528(int) { \
            return offset = 528; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset529(int) { \
            return offset = 529; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset530(int) { \
            return offset = 530; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset531(int) { \
            return offset = 531; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset532(int) { \
            return offset = 532; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset533(int) { \
            return offset = 533; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset534(int) { \
            return offset = 534; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset535(int) { \
            return offset = 535; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset536(int) { \
            return offset = 536; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset537(int) { \
            return offset = 537; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset538(int) { \
            return offset = 538; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset539(int) { \
            return offset = 539; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset540(int) { \
            return offset = 540; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset541(int) { \
            return offset = 541; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset542(int) { \
            return offset = 542; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset543(int) { \
            return offset = 543; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset544(int) { \
            return offset = 544; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset545(int) { \
            return offset = 545; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset546(int) { \
            return offset = 546; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset547(int) { \
            return offset = 547; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset548(int) { \
            return offset = 548; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset549(int) { \
            return offset = 549; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset550(int) { \
            return offset = 550; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset551(int) { \
            return offset = 551; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset552(int) { \
            return offset = 552; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset553(int) { \
            return offset = 553; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset554(int) { \
            return offset = 554; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset555(int) { \
            return offset = 555; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset556(int) { \
            return offset = 556; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset557(int) { \
            return offset = 557; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset558(int) { \
            return offset = 558; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset559(int) { \
            return offset = 559; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset560(int) { \
            return offset = 560; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset561(int) { \
            return offset = 561; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset562(int) { \
            return offset = 562; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset563(int) { \
            return offset = 563; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset564(int) { \
            return offset = 564; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset565(int) { \
            return offset = 565; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset566(int) { \
            return offset = 566; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset567(int) { \
            return offset = 567; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset568(int) { \
            return offset = 568; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset569(int) { \
            return offset = 569; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset570(int) { \
            return offset = 570; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset571(int) { \
            return offset = 571; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset572(int) { \
            return offset = 572; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset573(int) { \
            return offset = 573; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset574(int) { \
            return offset = 574; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset575(int) { \
            return offset = 575; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset576(int) { \
            return offset = 576; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset577(int) { \
            return offset = 577; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset578(int) { \
            return offset = 578; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset579(int) { \
            return offset = 579; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset580(int) { \
            return offset = 580; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset581(int) { \
            return offset = 581; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset582(int) { \
            return offset = 582; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset583(int) { \
            return offset = 583; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset584(int) { \
            return offset = 584; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset585(int) { \
            return offset = 585; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset586(int) { \
            return offset = 586; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset587(int) { \
            return offset = 587; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset588(int) { \
            return offset = 588; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset589(int) { \
            return offset = 589; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset590(int) { \
            return offset = 590; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset591(int) { \
            return offset = 591; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset592(int) { \
            return offset = 592; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset593(int) { \
            return offset = 593; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset594(int) { \
            return offset = 594; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset595(int) { \
            return offset = 595; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset596(int) { \
            return offset = 596; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset597(int) { \
            return offset = 597; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset598(int) { \
            return offset = 598; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset599(int) { \
            return offset = 599; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset600(int) { \
            return offset = 600; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset601(int) { \
            return offset = 601; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset602(int) { \
            return offset = 602; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset603(int) { \
            return offset = 603; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset604(int) { \
            return offset = 604; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset605(int) { \
            return offset = 605; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset606(int) { \
            return offset = 606; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset607(int) { \
            return offset = 607; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset608(int) { \
            return offset = 608; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset609(int) { \
            return offset = 609; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset610(int) { \
            return offset = 610; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset611(int) { \
            return offset = 611; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset612(int) { \
            return offset = 612; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset613(int) { \
            return offset = 613; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset614(int) { \
            return offset = 614; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset615(int) { \
            return offset = 615; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset616(int) { \
            return offset = 616; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset617(int) { \
            return offset = 617; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset618(int) { \
            return offset = 618; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset619(int) { \
            return offset = 619; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset620(int) { \
            return offset = 620; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset621(int) { \
            return offset = 621; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset622(int) { \
            return offset = 622; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset623(int) { \
            return offset = 623; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset624(int) { \
            return offset = 624; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset625(int) { \
            return offset = 625; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset626(int) { \
            return offset = 626; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset627(int) { \
            return offset = 627; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset628(int) { \
            return offset = 628; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset629(int) { \
            return offset = 629; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset630(int) { \
            return offset = 630; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset631(int) { \
            return offset = 631; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset632(int) { \
            return offset = 632; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset633(int) { \
            return offset = 633; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset634(int) { \
            return offset = 634; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset635(int) { \
            return offset = 635; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset636(int) { \
            return offset = 636; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset637(int) { \
            return offset = 637; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset638(int) { \
            return offset = 638; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset639(int) { \
            return offset = 639; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset640(int) { \
            return offset = 640; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset641(int) { \
            return offset = 641; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset642(int) { \
            return offset = 642; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset643(int) { \
            return offset = 643; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset644(int) { \
            return offset = 644; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset645(int) { \
            return offset = 645; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset646(int) { \
            return offset = 646; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset647(int) { \
            return offset = 647; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset648(int) { \
            return offset = 648; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset649(int) { \
            return offset = 649; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset650(int) { \
            return offset = 650; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset651(int) { \
            return offset = 651; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset652(int) { \
            return offset = 652; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset653(int) { \
            return offset = 653; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset654(int) { \
            return offset = 654; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset655(int) { \
            return offset = 655; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset656(int) { \
            return offset = 656; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset657(int) { \
            return offset = 657; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset658(int) { \
            return offset = 658; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset659(int) { \
            return offset = 659; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset660(int) { \
            return offset = 660; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset661(int) { \
            return offset = 661; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset662(int) { \
            return offset = 662; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset663(int) { \
            return offset = 663; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset664(int) { \
            return offset = 664; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset665(int) { \
            return offset = 665; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset666(int) { \
            return offset = 666; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset667(int) { \
            return offset = 667; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset668(int) { \
            return offset = 668; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset669(int) { \
            return offset = 669; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset670(int) { \
            return offset = 670; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset671(int) { \
            return offset = 671; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset672(int) { \
            return offset = 672; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset673(int) { \
            return offset = 673; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset674(int) { \
            return offset = 674; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset675(int) { \
            return offset = 675; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset676(int) { \
            return offset = 676; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset677(int) { \
            return offset = 677; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset678(int) { \
            return offset = 678; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset679(int) { \
            return offset = 679; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset680(int) { \
            return offset = 680; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset681(int) { \
            return offset = 681; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset682(int) { \
            return offset = 682; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset683(int) { \
            return offset = 683; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset684(int) { \
            return offset = 684; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset685(int) { \
            return offset = 685; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset686(int) { \
            return offset = 686; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset687(int) { \
            return offset = 687; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset688(int) { \
            return offset = 688; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset689(int) { \
            return offset = 689; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset690(int) { \
            return offset = 690; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset691(int) { \
            return offset = 691; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset692(int) { \
            return offset = 692; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset693(int) { \
            return offset = 693; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset694(int) { \
            return offset = 694; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset695(int) { \
            return offset = 695; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset696(int) { \
            return offset = 696; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset697(int) { \
            return offset = 697; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset698(int) { \
            return offset = 698; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset699(int) { \
            return offset = 699; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset700(int) { \
            return offset = 700; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset701(int) { \
            return offset = 701; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset702(int) { \
            return offset = 702; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset703(int) { \
            return offset = 703; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset704(int) { \
            return offset = 704; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset705(int) { \
            return offset = 705; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset706(int) { \
            return offset = 706; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset707(int) { \
            return offset = 707; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset708(int) { \
            return offset = 708; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset709(int) { \
            return offset = 709; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset710(int) { \
            return offset = 710; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset711(int) { \
            return offset = 711; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset712(int) { \
            return offset = 712; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset713(int) { \
            return offset = 713; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset714(int) { \
            return offset = 714; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset715(int) { \
            return offset = 715; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset716(int) { \
            return offset = 716; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset717(int) { \
            return offset = 717; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset718(int) { \
            return offset = 718; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset719(int) { \
            return offset = 719; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset720(int) { \
            return offset = 720; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset721(int) { \
            return offset = 721; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset722(int) { \
            return offset = 722; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset723(int) { \
            return offset = 723; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset724(int) { \
            return offset = 724; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset725(int) { \
            return offset = 725; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset726(int) { \
            return offset = 726; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset727(int) { \
            return offset = 727; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset728(int) { \
            return offset = 728; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset729(int) { \
            return offset = 729; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset730(int) { \
            return offset = 730; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset731(int) { \
            return offset = 731; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset732(int) { \
            return offset = 732; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset733(int) { \
            return offset = 733; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset734(int) { \
            return offset = 734; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset735(int) { \
            return offset = 735; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset736(int) { \
            return offset = 736; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset737(int) { \
            return offset = 737; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset738(int) { \
            return offset = 738; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset739(int) { \
            return offset = 739; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset740(int) { \
            return offset = 740; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset741(int) { \
            return offset = 741; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset742(int) { \
            return offset = 742; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset743(int) { \
            return offset = 743; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset744(int) { \
            return offset = 744; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset745(int) { \
            return offset = 745; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset746(int) { \
            return offset = 746; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset747(int) { \
            return offset = 747; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset748(int) { \
            return offset = 748; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset749(int) { \
            return offset = 749; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset750(int) { \
            return offset = 750; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset751(int) { \
            return offset = 751; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset752(int) { \
            return offset = 752; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset753(int) { \
            return offset = 753; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset754(int) { \
            return offset = 754; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset755(int) { \
            return offset = 755; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset756(int) { \
            return offset = 756; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset757(int) { \
            return offset = 757; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset758(int) { \
            return offset = 758; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset759(int) { \
            return offset = 759; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset760(int) { \
            return offset = 760; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset761(int) { \
            return offset = 761; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset762(int) { \
            return offset = 762; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset763(int) { \
            return offset = 763; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset764(int) { \
            return offset = 764; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset765(int) { \
            return offset = 765; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset766(int) { \
            return offset = 766; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset767(int) { \
            return offset = 767; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset768(int) { \
            return offset = 768; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset769(int) { \
            return offset = 769; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset770(int) { \
            return offset = 770; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset771(int) { \
            return offset = 771; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset772(int) { \
            return offset = 772; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset773(int) { \
            return offset = 773; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset774(int) { \
            return offset = 774; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset775(int) { \
            return offset = 775; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset776(int) { \
            return offset = 776; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset777(int) { \
            return offset = 777; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset778(int) { \
            return offset = 778; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset779(int) { \
            return offset = 779; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset780(int) { \
            return offset = 780; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset781(int) { \
            return offset = 781; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset782(int) { \
            return offset = 782; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset783(int) { \
            return offset = 783; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset784(int) { \
            return offset = 784; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset785(int) { \
            return offset = 785; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset786(int) { \
            return offset = 786; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset787(int) { \
            return offset = 787; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset788(int) { \
            return offset = 788; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset789(int) { \
            return offset = 789; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset790(int) { \
            return offset = 790; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset791(int) { \
            return offset = 791; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset792(int) { \
            return offset = 792; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset793(int) { \
            return offset = 793; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset794(int) { \
            return offset = 794; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset795(int) { \
            return offset = 795; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset796(int) { \
            return offset = 796; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset797(int) { \
            return offset = 797; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset798(int) { \
            return offset = 798; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset799(int) { \
            return offset = 799; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset800(int) { \
            return offset = 800; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset801(int) { \
            return offset = 801; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset802(int) { \
            return offset = 802; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset803(int) { \
            return offset = 803; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset804(int) { \
            return offset = 804; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset805(int) { \
            return offset = 805; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset806(int) { \
            return offset = 806; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset807(int) { \
            return offset = 807; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset808(int) { \
            return offset = 808; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset809(int) { \
            return offset = 809; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset810(int) { \
            return offset = 810; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset811(int) { \
            return offset = 811; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset812(int) { \
            return offset = 812; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset813(int) { \
            return offset = 813; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset814(int) { \
            return offset = 814; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset815(int) { \
            return offset = 815; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset816(int) { \
            return offset = 816; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset817(int) { \
            return offset = 817; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset818(int) { \
            return offset = 818; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset819(int) { \
            return offset = 819; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset820(int) { \
            return offset = 820; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset821(int) { \
            return offset = 821; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset822(int) { \
            return offset = 822; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset823(int) { \
            return offset = 823; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset824(int) { \
            return offset = 824; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset825(int) { \
            return offset = 825; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset826(int) { \
            return offset = 826; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset827(int) { \
            return offset = 827; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset828(int) { \
            return offset = 828; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset829(int) { \
            return offset = 829; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset830(int) { \
            return offset = 830; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset831(int) { \
            return offset = 831; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset832(int) { \
            return offset = 832; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset833(int) { \
            return offset = 833; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset834(int) { \
            return offset = 834; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset835(int) { \
            return offset = 835; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset836(int) { \
            return offset = 836; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset837(int) { \
            return offset = 837; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset838(int) { \
            return offset = 838; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset839(int) { \
            return offset = 839; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset840(int) { \
            return offset = 840; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset841(int) { \
            return offset = 841; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset842(int) { \
            return offset = 842; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset843(int) { \
            return offset = 843; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset844(int) { \
            return offset = 844; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset845(int) { \
            return offset = 845; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset846(int) { \
            return offset = 846; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset847(int) { \
            return offset = 847; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset848(int) { \
            return offset = 848; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset849(int) { \
            return offset = 849; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset850(int) { \
            return offset = 850; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset851(int) { \
            return offset = 851; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset852(int) { \
            return offset = 852; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset853(int) { \
            return offset = 853; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset854(int) { \
            return offset = 854; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset855(int) { \
            return offset = 855; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset856(int) { \
            return offset = 856; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset857(int) { \
            return offset = 857; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset858(int) { \
            return offset = 858; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset859(int) { \
            return offset = 859; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset860(int) { \
            return offset = 860; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset861(int) { \
            return offset = 861; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset862(int) { \
            return offset = 862; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset863(int) { \
            return offset = 863; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset864(int) { \
            return offset = 864; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset865(int) { \
            return offset = 865; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset866(int) { \
            return offset = 866; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset867(int) { \
            return offset = 867; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset868(int) { \
            return offset = 868; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset869(int) { \
            return offset = 869; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset870(int) { \
            return offset = 870; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset871(int) { \
            return offset = 871; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset872(int) { \
            return offset = 872; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset873(int) { \
            return offset = 873; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset874(int) { \
            return offset = 874; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset875(int) { \
            return offset = 875; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset876(int) { \
            return offset = 876; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset877(int) { \
            return offset = 877; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset878(int) { \
            return offset = 878; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset879(int) { \
            return offset = 879; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset880(int) { \
            return offset = 880; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset881(int) { \
            return offset = 881; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset882(int) { \
            return offset = 882; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset883(int) { \
            return offset = 883; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset884(int) { \
            return offset = 884; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset885(int) { \
            return offset = 885; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset886(int) { \
            return offset = 886; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset887(int) { \
            return offset = 887; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset888(int) { \
            return offset = 888; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset889(int) { \
            return offset = 889; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset890(int) { \
            return offset = 890; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset891(int) { \
            return offset = 891; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset892(int) { \
            return offset = 892; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset893(int) { \
            return offset = 893; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset894(int) { \
            return offset = 894; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset895(int) { \
            return offset = 895; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset896(int) { \
            return offset = 896; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset897(int) { \
            return offset = 897; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset898(int) { \
            return offset = 898; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset899(int) { \
            return offset = 899; \
        } \
 \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset900(int) { \
            return offset = 900; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset901(int) { \
            return offset = 901; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset902(int) { \
            return offset = 902; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset903(int) { \
            return offset = 903; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset904(int) { \
            return offset = 904; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset905(int) { \
            return offset = 905; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset906(int) { \
            return offset = 906; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset907(int) { \
            return offset = 907; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset908(int) { \
            return offset = 908; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset909(int) { \
            return offset = 909; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset910(int) { \
            return offset = 910; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset911(int) { \
            return offset = 911; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset912(int) { \
            return offset = 912; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset913(int) { \
            return offset = 913; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset914(int) { \
            return offset = 914; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset915(int) { \
            return offset = 915; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset916(int) { \
            return offset = 916; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset917(int) { \
            return offset = 917; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset918(int) { \
            return offset = 918; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset919(int) { \
            return offset = 919; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset920(int) { \
            return offset = 920; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset921(int) { \
            return offset = 921; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset922(int) { \
            return offset = 922; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset923(int) { \
            return offset = 923; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset924(int) { \
            return offset = 924; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset925(int) { \
            return offset = 925; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset926(int) { \
            return offset = 926; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset927(int) { \
            return offset = 927; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset928(int) { \
            return offset = 928; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset929(int) { \
            return offset = 929; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset930(int) { \
            return offset = 930; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset931(int) { \
            return offset = 931; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset932(int) { \
            return offset = 932; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset933(int) { \
            return offset = 933; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset934(int) { \
            return offset = 934; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset935(int) { \
            return offset = 935; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset936(int) { \
            return offset = 936; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset937(int) { \
            return offset = 937; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset938(int) { \
            return offset = 938; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset939(int) { \
            return offset = 939; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset940(int) { \
            return offset = 940; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset941(int) { \
            return offset = 941; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset942(int) { \
            return offset = 942; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset943(int) { \
            return offset = 943; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset944(int) { \
            return offset = 944; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset945(int) { \
            return offset = 945; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset946(int) { \
            return offset = 946; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset947(int) { \
            return offset = 947; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset948(int) { \
            return offset = 948; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset949(int) { \
            return offset = 949; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset950(int) { \
            return offset = 950; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset951(int) { \
            return offset = 951; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset952(int) { \
            return offset = 952; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset953(int) { \
            return offset = 953; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset954(int) { \
            return offset = 954; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset955(int) { \
            return offset = 955; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset956(int) { \
            return offset = 956; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset957(int) { \
            return offset = 957; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset958(int) { \
            return offset = 958; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset959(int) { \
            return offset = 959; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset960(int) { \
            return offset = 960; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset961(int) { \
            return offset = 961; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset962(int) { \
            return offset = 962; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset963(int) { \
            return offset = 963; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset964(int) { \
            return offset = 964; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset965(int) { \
            return offset = 965; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset966(int) { \
            return offset = 966; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset967(int) { \
            return offset = 967; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset968(int) { \
            return offset = 968; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset969(int) { \
            return offset = 969; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset970(int) { \
            return offset = 970; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset971(int) { \
            return offset = 971; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset972(int) { \
            return offset = 972; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset973(int) { \
            return offset = 973; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset974(int) { \
            return offset = 974; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset975(int) { \
            return offset = 975; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset976(int) { \
            return offset = 976; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset977(int) { \
            return offset = 977; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset978(int) { \
            return offset = 978; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset979(int) { \
            return offset = 979; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset980(int) { \
            return offset = 980; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset981(int) { \
            return offset = 981; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset982(int) { \
            return offset = 982; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset983(int) { \
            return offset = 983; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset984(int) { \
            return offset = 984; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset985(int) { \
            return offset = 985; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset986(int) { \
            return offset = 986; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset987(int) { \
            return offset = 987; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset988(int) { \
            return offset = 988; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset989(int) { \
            return offset = 989; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset990(int) { \
            return offset = 990; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset991(int) { \
            return offset = 991; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset992(int) { \
            return offset = 992; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset993(int) { \
            return offset = 993; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset994(int) { \
            return offset = 994; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset995(int) { \
            return offset = 995; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset996(int) { \
            return offset = 996; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset997(int) { \
            return offset = 997; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset998(int) { \
            return offset = 998; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset999(int) { \
            return offset = 999; \
        } \
 \
        virtual unsigned int CONVENTION_SPECIFIER offset1000(int) { \
            return offset = 1000; \
        } \
 \
    };

	FAKEIT_MAKE_SELECTOR( Thiscall, __thiscall)
	FAKEIT_MAKE_SELECTOR( Stdcall, __stdcall )
	FAKEIT_MAKE_SELECTOR( Cdecl, __cdecl )
}
