/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "fakeit/FakeitExceptions.hpp"

using namespace fakeit;

struct VirtualOffsetSelectorTest : tpunit::TestFixture {
    VirtualOffsetSelectorTest() :
            tpunit::TestFixture(
                    //
                    TEST(VirtualOffsetSelectorTest::verifyAllIndexes)
                    //
            ) {
    }


    void verifyAllIndexes() {
        VirtualOffsetSelector os;
        ASSERT_EQUAL(0, os.offset0(0));
        ASSERT_EQUAL(1, os.offset1(0));
        ASSERT_EQUAL(2, os.offset2(0));
        ASSERT_EQUAL(3, os.offset3(0));
        ASSERT_EQUAL(4, os.offset4(0));
        ASSERT_EQUAL(5, os.offset5(0));
        ASSERT_EQUAL(6, os.offset6(0));
        ASSERT_EQUAL(7, os.offset7(0));
        ASSERT_EQUAL(8, os.offset8(0));
        ASSERT_EQUAL(9, os.offset9(0));

        ASSERT_EQUAL(10, os.offset10(0));
        ASSERT_EQUAL(11, os.offset11(0));
        ASSERT_EQUAL(12, os.offset12(0));
        ASSERT_EQUAL(13, os.offset13(0));
        ASSERT_EQUAL(14, os.offset14(0));
        ASSERT_EQUAL(15, os.offset15(0));
        ASSERT_EQUAL(16, os.offset16(0));
        ASSERT_EQUAL(17, os.offset17(0));
        ASSERT_EQUAL(18, os.offset18(0));
        ASSERT_EQUAL(19, os.offset19(0));

        ASSERT_EQUAL(20, os.offset20(0));
        ASSERT_EQUAL(21, os.offset21(0));
        ASSERT_EQUAL(22, os.offset22(0));
        ASSERT_EQUAL(23, os.offset23(0));
        ASSERT_EQUAL(24, os.offset24(0));
        ASSERT_EQUAL(25, os.offset25(0));
        ASSERT_EQUAL(26, os.offset26(0));
        ASSERT_EQUAL(27, os.offset27(0));
        ASSERT_EQUAL(28, os.offset28(0));
        ASSERT_EQUAL(29, os.offset29(0));

        ASSERT_EQUAL(30, os.offset30(0));
        ASSERT_EQUAL(31, os.offset31(0));
        ASSERT_EQUAL(32, os.offset32(0));
        ASSERT_EQUAL(33, os.offset33(0));
        ASSERT_EQUAL(34, os.offset34(0));
        ASSERT_EQUAL(35, os.offset35(0));
        ASSERT_EQUAL(36, os.offset36(0));
        ASSERT_EQUAL(37, os.offset37(0));
        ASSERT_EQUAL(38, os.offset38(0));
        ASSERT_EQUAL(39, os.offset39(0));

        ASSERT_EQUAL(40, os.offset40(0));
        ASSERT_EQUAL(41, os.offset41(0));
        ASSERT_EQUAL(42, os.offset42(0));
        ASSERT_EQUAL(43, os.offset43(0));
        ASSERT_EQUAL(44, os.offset44(0));
        ASSERT_EQUAL(45, os.offset45(0));
        ASSERT_EQUAL(46, os.offset46(0));
        ASSERT_EQUAL(47, os.offset47(0));
        ASSERT_EQUAL(48, os.offset48(0));
        ASSERT_EQUAL(49, os.offset49(0));

        ASSERT_EQUAL(50, os.offset50(0));
        ASSERT_EQUAL(51, os.offset51(0));
        ASSERT_EQUAL(52, os.offset52(0));
        ASSERT_EQUAL(53, os.offset53(0));
        ASSERT_EQUAL(54, os.offset54(0));
        ASSERT_EQUAL(55, os.offset55(0));
        ASSERT_EQUAL(56, os.offset56(0));
        ASSERT_EQUAL(57, os.offset57(0));
        ASSERT_EQUAL(58, os.offset58(0));
        ASSERT_EQUAL(59, os.offset59(0));

        ASSERT_EQUAL(60, os.offset60(0));
        ASSERT_EQUAL(61, os.offset61(0));
        ASSERT_EQUAL(62, os.offset62(0));
        ASSERT_EQUAL(63, os.offset63(0));
        ASSERT_EQUAL(64, os.offset64(0));
        ASSERT_EQUAL(65, os.offset65(0));
        ASSERT_EQUAL(66, os.offset66(0));
        ASSERT_EQUAL(67, os.offset67(0));
        ASSERT_EQUAL(68, os.offset68(0));
        ASSERT_EQUAL(69, os.offset69(0));

        ASSERT_EQUAL(70, os.offset70(0));
        ASSERT_EQUAL(71, os.offset71(0));
        ASSERT_EQUAL(72, os.offset72(0));
        ASSERT_EQUAL(73, os.offset73(0));
        ASSERT_EQUAL(74, os.offset74(0));
        ASSERT_EQUAL(75, os.offset75(0));
        ASSERT_EQUAL(76, os.offset76(0));
        ASSERT_EQUAL(77, os.offset77(0));
        ASSERT_EQUAL(78, os.offset78(0));
        ASSERT_EQUAL(79, os.offset79(0));

        ASSERT_EQUAL(80, os.offset80(0));
        ASSERT_EQUAL(81, os.offset81(0));
        ASSERT_EQUAL(82, os.offset82(0));
        ASSERT_EQUAL(83, os.offset83(0));
        ASSERT_EQUAL(84, os.offset84(0));
        ASSERT_EQUAL(85, os.offset85(0));
        ASSERT_EQUAL(86, os.offset86(0));
        ASSERT_EQUAL(87, os.offset87(0));
        ASSERT_EQUAL(88, os.offset88(0));
        ASSERT_EQUAL(89, os.offset89(0));

        ASSERT_EQUAL(90, os.offset90(0));
        ASSERT_EQUAL(91, os.offset91(0));
        ASSERT_EQUAL(92, os.offset92(0));
        ASSERT_EQUAL(93, os.offset93(0));
        ASSERT_EQUAL(94, os.offset94(0));
        ASSERT_EQUAL(95, os.offset95(0));
        ASSERT_EQUAL(96, os.offset96(0));
        ASSERT_EQUAL(97, os.offset97(0));
        ASSERT_EQUAL(98, os.offset98(0));
        ASSERT_EQUAL(99, os.offset99(0));

        ASSERT_EQUAL(100, os.offset100(0));
        ASSERT_EQUAL(101, os.offset101(0));
        ASSERT_EQUAL(102, os.offset102(0));
        ASSERT_EQUAL(103, os.offset103(0));
        ASSERT_EQUAL(104, os.offset104(0));
        ASSERT_EQUAL(105, os.offset105(0));
        ASSERT_EQUAL(106, os.offset106(0));
        ASSERT_EQUAL(107, os.offset107(0));
        ASSERT_EQUAL(108, os.offset108(0));
        ASSERT_EQUAL(109, os.offset109(0));

        ASSERT_EQUAL(110, os.offset110(0));
        ASSERT_EQUAL(111, os.offset111(0));
        ASSERT_EQUAL(112, os.offset112(0));
        ASSERT_EQUAL(113, os.offset113(0));
        ASSERT_EQUAL(114, os.offset114(0));
        ASSERT_EQUAL(115, os.offset115(0));
        ASSERT_EQUAL(116, os.offset116(0));
        ASSERT_EQUAL(117, os.offset117(0));
        ASSERT_EQUAL(118, os.offset118(0));
        ASSERT_EQUAL(119, os.offset119(0));

        ASSERT_EQUAL(120, os.offset120(0));
        ASSERT_EQUAL(121, os.offset121(0));
        ASSERT_EQUAL(122, os.offset122(0));
        ASSERT_EQUAL(123, os.offset123(0));
        ASSERT_EQUAL(124, os.offset124(0));
        ASSERT_EQUAL(125, os.offset125(0));
        ASSERT_EQUAL(126, os.offset126(0));
        ASSERT_EQUAL(127, os.offset127(0));
        ASSERT_EQUAL(128, os.offset128(0));
        ASSERT_EQUAL(129, os.offset129(0));

        ASSERT_EQUAL(130, os.offset130(0));
        ASSERT_EQUAL(131, os.offset131(0));
        ASSERT_EQUAL(132, os.offset132(0));
        ASSERT_EQUAL(133, os.offset133(0));
        ASSERT_EQUAL(134, os.offset134(0));
        ASSERT_EQUAL(135, os.offset135(0));
        ASSERT_EQUAL(136, os.offset136(0));
        ASSERT_EQUAL(137, os.offset137(0));
        ASSERT_EQUAL(138, os.offset138(0));
        ASSERT_EQUAL(139, os.offset139(0));

        ASSERT_EQUAL(140, os.offset140(0));
        ASSERT_EQUAL(141, os.offset141(0));
        ASSERT_EQUAL(142, os.offset142(0));
        ASSERT_EQUAL(143, os.offset143(0));
        ASSERT_EQUAL(144, os.offset144(0));
        ASSERT_EQUAL(145, os.offset145(0));
        ASSERT_EQUAL(146, os.offset146(0));
        ASSERT_EQUAL(147, os.offset147(0));
        ASSERT_EQUAL(148, os.offset148(0));
        ASSERT_EQUAL(149, os.offset149(0));

        ASSERT_EQUAL(150, os.offset150(0));
        ASSERT_EQUAL(151, os.offset151(0));
        ASSERT_EQUAL(152, os.offset152(0));
        ASSERT_EQUAL(153, os.offset153(0));
        ASSERT_EQUAL(154, os.offset154(0));
        ASSERT_EQUAL(155, os.offset155(0));
        ASSERT_EQUAL(156, os.offset156(0));
        ASSERT_EQUAL(157, os.offset157(0));
        ASSERT_EQUAL(158, os.offset158(0));
        ASSERT_EQUAL(159, os.offset159(0));

        ASSERT_EQUAL(160, os.offset160(0));
        ASSERT_EQUAL(161, os.offset161(0));
        ASSERT_EQUAL(162, os.offset162(0));
        ASSERT_EQUAL(163, os.offset163(0));
        ASSERT_EQUAL(164, os.offset164(0));
        ASSERT_EQUAL(165, os.offset165(0));
        ASSERT_EQUAL(166, os.offset166(0));
        ASSERT_EQUAL(167, os.offset167(0));
        ASSERT_EQUAL(168, os.offset168(0));
        ASSERT_EQUAL(169, os.offset169(0));

        ASSERT_EQUAL(170, os.offset170(0));
        ASSERT_EQUAL(171, os.offset171(0));
        ASSERT_EQUAL(172, os.offset172(0));
        ASSERT_EQUAL(173, os.offset173(0));
        ASSERT_EQUAL(174, os.offset174(0));
        ASSERT_EQUAL(175, os.offset175(0));
        ASSERT_EQUAL(176, os.offset176(0));
        ASSERT_EQUAL(177, os.offset177(0));
        ASSERT_EQUAL(178, os.offset178(0));
        ASSERT_EQUAL(179, os.offset179(0));

        ASSERT_EQUAL(180, os.offset180(0));
        ASSERT_EQUAL(181, os.offset181(0));
        ASSERT_EQUAL(182, os.offset182(0));
        ASSERT_EQUAL(183, os.offset183(0));
        ASSERT_EQUAL(184, os.offset184(0));
        ASSERT_EQUAL(185, os.offset185(0));
        ASSERT_EQUAL(186, os.offset186(0));
        ASSERT_EQUAL(187, os.offset187(0));
        ASSERT_EQUAL(188, os.offset188(0));
        ASSERT_EQUAL(189, os.offset189(0));

        ASSERT_EQUAL(190, os.offset190(0));
        ASSERT_EQUAL(191, os.offset191(0));
        ASSERT_EQUAL(192, os.offset192(0));
        ASSERT_EQUAL(193, os.offset193(0));
        ASSERT_EQUAL(194, os.offset194(0));
        ASSERT_EQUAL(195, os.offset195(0));
        ASSERT_EQUAL(196, os.offset196(0));
        ASSERT_EQUAL(197, os.offset197(0));
        ASSERT_EQUAL(198, os.offset198(0));
        ASSERT_EQUAL(199, os.offset199(0));


        ASSERT_EQUAL(200, os.offset200(0));
        ASSERT_EQUAL(201, os.offset201(0));
        ASSERT_EQUAL(202, os.offset202(0));
        ASSERT_EQUAL(203, os.offset203(0));
        ASSERT_EQUAL(204, os.offset204(0));
        ASSERT_EQUAL(205, os.offset205(0));
        ASSERT_EQUAL(206, os.offset206(0));
        ASSERT_EQUAL(207, os.offset207(0));
        ASSERT_EQUAL(208, os.offset208(0));
        ASSERT_EQUAL(209, os.offset209(0));

        ASSERT_EQUAL(210, os.offset210(0));
        ASSERT_EQUAL(211, os.offset211(0));
        ASSERT_EQUAL(212, os.offset212(0));
        ASSERT_EQUAL(213, os.offset213(0));
        ASSERT_EQUAL(214, os.offset214(0));
        ASSERT_EQUAL(215, os.offset215(0));
        ASSERT_EQUAL(216, os.offset216(0));
        ASSERT_EQUAL(217, os.offset217(0));
        ASSERT_EQUAL(218, os.offset218(0));
        ASSERT_EQUAL(219, os.offset219(0));

        ASSERT_EQUAL(220, os.offset220(0));
        ASSERT_EQUAL(221, os.offset221(0));
        ASSERT_EQUAL(222, os.offset222(0));
        ASSERT_EQUAL(223, os.offset223(0));
        ASSERT_EQUAL(224, os.offset224(0));
        ASSERT_EQUAL(225, os.offset225(0));
        ASSERT_EQUAL(226, os.offset226(0));
        ASSERT_EQUAL(227, os.offset227(0));
        ASSERT_EQUAL(228, os.offset228(0));
        ASSERT_EQUAL(229, os.offset229(0));

        ASSERT_EQUAL(230, os.offset230(0));
        ASSERT_EQUAL(231, os.offset231(0));
        ASSERT_EQUAL(232, os.offset232(0));
        ASSERT_EQUAL(233, os.offset233(0));
        ASSERT_EQUAL(234, os.offset234(0));
        ASSERT_EQUAL(235, os.offset235(0));
        ASSERT_EQUAL(236, os.offset236(0));
        ASSERT_EQUAL(237, os.offset237(0));
        ASSERT_EQUAL(238, os.offset238(0));
        ASSERT_EQUAL(239, os.offset239(0));

        ASSERT_EQUAL(240, os.offset240(0));
        ASSERT_EQUAL(241, os.offset241(0));
        ASSERT_EQUAL(242, os.offset242(0));
        ASSERT_EQUAL(243, os.offset243(0));
        ASSERT_EQUAL(244, os.offset244(0));
        ASSERT_EQUAL(245, os.offset245(0));
        ASSERT_EQUAL(246, os.offset246(0));
        ASSERT_EQUAL(247, os.offset247(0));
        ASSERT_EQUAL(248, os.offset248(0));
        ASSERT_EQUAL(249, os.offset249(0));

        ASSERT_EQUAL(250, os.offset250(0));
        ASSERT_EQUAL(251, os.offset251(0));
        ASSERT_EQUAL(252, os.offset252(0));
        ASSERT_EQUAL(253, os.offset253(0));
        ASSERT_EQUAL(254, os.offset254(0));
        ASSERT_EQUAL(255, os.offset255(0));
        ASSERT_EQUAL(256, os.offset256(0));
        ASSERT_EQUAL(257, os.offset257(0));
        ASSERT_EQUAL(258, os.offset258(0));
        ASSERT_EQUAL(259, os.offset259(0));

        ASSERT_EQUAL(260, os.offset260(0));
        ASSERT_EQUAL(261, os.offset261(0));
        ASSERT_EQUAL(262, os.offset262(0));
        ASSERT_EQUAL(263, os.offset263(0));
        ASSERT_EQUAL(264, os.offset264(0));
        ASSERT_EQUAL(265, os.offset265(0));
        ASSERT_EQUAL(266, os.offset266(0));
        ASSERT_EQUAL(267, os.offset267(0));
        ASSERT_EQUAL(268, os.offset268(0));
        ASSERT_EQUAL(269, os.offset269(0));

        ASSERT_EQUAL(270, os.offset270(0));
        ASSERT_EQUAL(271, os.offset271(0));
        ASSERT_EQUAL(272, os.offset272(0));
        ASSERT_EQUAL(273, os.offset273(0));
        ASSERT_EQUAL(274, os.offset274(0));
        ASSERT_EQUAL(275, os.offset275(0));
        ASSERT_EQUAL(276, os.offset276(0));
        ASSERT_EQUAL(277, os.offset277(0));
        ASSERT_EQUAL(278, os.offset278(0));
        ASSERT_EQUAL(279, os.offset279(0));

        ASSERT_EQUAL(280, os.offset280(0));
        ASSERT_EQUAL(281, os.offset281(0));
        ASSERT_EQUAL(282, os.offset282(0));
        ASSERT_EQUAL(283, os.offset283(0));
        ASSERT_EQUAL(284, os.offset284(0));
        ASSERT_EQUAL(285, os.offset285(0));
        ASSERT_EQUAL(286, os.offset286(0));
        ASSERT_EQUAL(287, os.offset287(0));
        ASSERT_EQUAL(288, os.offset288(0));
        ASSERT_EQUAL(289, os.offset289(0));

        ASSERT_EQUAL(290, os.offset290(0));
        ASSERT_EQUAL(291, os.offset291(0));
        ASSERT_EQUAL(292, os.offset292(0));
        ASSERT_EQUAL(293, os.offset293(0));
        ASSERT_EQUAL(294, os.offset294(0));
        ASSERT_EQUAL(295, os.offset295(0));
        ASSERT_EQUAL(296, os.offset296(0));
        ASSERT_EQUAL(297, os.offset297(0));
        ASSERT_EQUAL(298, os.offset298(0));
        ASSERT_EQUAL(299, os.offset299(0));


        ASSERT_EQUAL(300, os.offset300(0));
        ASSERT_EQUAL(301, os.offset301(0));
        ASSERT_EQUAL(302, os.offset302(0));
        ASSERT_EQUAL(303, os.offset303(0));
        ASSERT_EQUAL(304, os.offset304(0));
        ASSERT_EQUAL(305, os.offset305(0));
        ASSERT_EQUAL(306, os.offset306(0));
        ASSERT_EQUAL(307, os.offset307(0));
        ASSERT_EQUAL(308, os.offset308(0));
        ASSERT_EQUAL(309, os.offset309(0));

        ASSERT_EQUAL(310, os.offset310(0));
        ASSERT_EQUAL(311, os.offset311(0));
        ASSERT_EQUAL(312, os.offset312(0));
        ASSERT_EQUAL(313, os.offset313(0));
        ASSERT_EQUAL(314, os.offset314(0));
        ASSERT_EQUAL(315, os.offset315(0));
        ASSERT_EQUAL(316, os.offset316(0));
        ASSERT_EQUAL(317, os.offset317(0));
        ASSERT_EQUAL(318, os.offset318(0));
        ASSERT_EQUAL(319, os.offset319(0));

        ASSERT_EQUAL(320, os.offset320(0));
        ASSERT_EQUAL(321, os.offset321(0));
        ASSERT_EQUAL(322, os.offset322(0));
        ASSERT_EQUAL(323, os.offset323(0));
        ASSERT_EQUAL(324, os.offset324(0));
        ASSERT_EQUAL(325, os.offset325(0));
        ASSERT_EQUAL(326, os.offset326(0));
        ASSERT_EQUAL(327, os.offset327(0));
        ASSERT_EQUAL(328, os.offset328(0));
        ASSERT_EQUAL(329, os.offset329(0));

        ASSERT_EQUAL(330, os.offset330(0));
        ASSERT_EQUAL(331, os.offset331(0));
        ASSERT_EQUAL(332, os.offset332(0));
        ASSERT_EQUAL(333, os.offset333(0));
        ASSERT_EQUAL(334, os.offset334(0));
        ASSERT_EQUAL(335, os.offset335(0));
        ASSERT_EQUAL(336, os.offset336(0));
        ASSERT_EQUAL(337, os.offset337(0));
        ASSERT_EQUAL(338, os.offset338(0));
        ASSERT_EQUAL(339, os.offset339(0));

        ASSERT_EQUAL(340, os.offset340(0));
        ASSERT_EQUAL(341, os.offset341(0));
        ASSERT_EQUAL(342, os.offset342(0));
        ASSERT_EQUAL(343, os.offset343(0));
        ASSERT_EQUAL(344, os.offset344(0));
        ASSERT_EQUAL(345, os.offset345(0));
        ASSERT_EQUAL(346, os.offset346(0));
        ASSERT_EQUAL(347, os.offset347(0));
        ASSERT_EQUAL(348, os.offset348(0));
        ASSERT_EQUAL(349, os.offset349(0));

        ASSERT_EQUAL(350, os.offset350(0));
        ASSERT_EQUAL(351, os.offset351(0));
        ASSERT_EQUAL(352, os.offset352(0));
        ASSERT_EQUAL(353, os.offset353(0));
        ASSERT_EQUAL(354, os.offset354(0));
        ASSERT_EQUAL(355, os.offset355(0));
        ASSERT_EQUAL(356, os.offset356(0));
        ASSERT_EQUAL(357, os.offset357(0));
        ASSERT_EQUAL(358, os.offset358(0));
        ASSERT_EQUAL(359, os.offset359(0));

        ASSERT_EQUAL(360, os.offset360(0));
        ASSERT_EQUAL(361, os.offset361(0));
        ASSERT_EQUAL(362, os.offset362(0));
        ASSERT_EQUAL(363, os.offset363(0));
        ASSERT_EQUAL(364, os.offset364(0));
        ASSERT_EQUAL(365, os.offset365(0));
        ASSERT_EQUAL(366, os.offset366(0));
        ASSERT_EQUAL(367, os.offset367(0));
        ASSERT_EQUAL(368, os.offset368(0));
        ASSERT_EQUAL(369, os.offset369(0));

        ASSERT_EQUAL(370, os.offset370(0));
        ASSERT_EQUAL(371, os.offset371(0));
        ASSERT_EQUAL(372, os.offset372(0));
        ASSERT_EQUAL(373, os.offset373(0));
        ASSERT_EQUAL(374, os.offset374(0));
        ASSERT_EQUAL(375, os.offset375(0));
        ASSERT_EQUAL(376, os.offset376(0));
        ASSERT_EQUAL(377, os.offset377(0));
        ASSERT_EQUAL(378, os.offset378(0));
        ASSERT_EQUAL(379, os.offset379(0));

        ASSERT_EQUAL(380, os.offset380(0));
        ASSERT_EQUAL(381, os.offset381(0));
        ASSERT_EQUAL(382, os.offset382(0));
        ASSERT_EQUAL(383, os.offset383(0));
        ASSERT_EQUAL(384, os.offset384(0));
        ASSERT_EQUAL(385, os.offset385(0));
        ASSERT_EQUAL(386, os.offset386(0));
        ASSERT_EQUAL(387, os.offset387(0));
        ASSERT_EQUAL(388, os.offset388(0));
        ASSERT_EQUAL(389, os.offset389(0));

        ASSERT_EQUAL(390, os.offset390(0));
        ASSERT_EQUAL(391, os.offset391(0));
        ASSERT_EQUAL(392, os.offset392(0));
        ASSERT_EQUAL(393, os.offset393(0));
        ASSERT_EQUAL(394, os.offset394(0));
        ASSERT_EQUAL(395, os.offset395(0));
        ASSERT_EQUAL(396, os.offset396(0));
        ASSERT_EQUAL(397, os.offset397(0));
        ASSERT_EQUAL(398, os.offset398(0));
        ASSERT_EQUAL(399, os.offset399(0));


        ASSERT_EQUAL(400, os.offset400(0));
        ASSERT_EQUAL(401, os.offset401(0));
        ASSERT_EQUAL(402, os.offset402(0));
        ASSERT_EQUAL(403, os.offset403(0));
        ASSERT_EQUAL(404, os.offset404(0));
        ASSERT_EQUAL(405, os.offset405(0));
        ASSERT_EQUAL(406, os.offset406(0));
        ASSERT_EQUAL(407, os.offset407(0));
        ASSERT_EQUAL(408, os.offset408(0));
        ASSERT_EQUAL(409, os.offset409(0));

        ASSERT_EQUAL(410, os.offset410(0));
        ASSERT_EQUAL(411, os.offset411(0));
        ASSERT_EQUAL(412, os.offset412(0));
        ASSERT_EQUAL(413, os.offset413(0));
        ASSERT_EQUAL(414, os.offset414(0));
        ASSERT_EQUAL(415, os.offset415(0));
        ASSERT_EQUAL(416, os.offset416(0));
        ASSERT_EQUAL(417, os.offset417(0));
        ASSERT_EQUAL(418, os.offset418(0));
        ASSERT_EQUAL(419, os.offset419(0));

        ASSERT_EQUAL(420, os.offset420(0));
        ASSERT_EQUAL(421, os.offset421(0));
        ASSERT_EQUAL(422, os.offset422(0));
        ASSERT_EQUAL(423, os.offset423(0));
        ASSERT_EQUAL(424, os.offset424(0));
        ASSERT_EQUAL(425, os.offset425(0));
        ASSERT_EQUAL(426, os.offset426(0));
        ASSERT_EQUAL(427, os.offset427(0));
        ASSERT_EQUAL(428, os.offset428(0));
        ASSERT_EQUAL(429, os.offset429(0));

        ASSERT_EQUAL(430, os.offset430(0));
        ASSERT_EQUAL(431, os.offset431(0));
        ASSERT_EQUAL(432, os.offset432(0));
        ASSERT_EQUAL(433, os.offset433(0));
        ASSERT_EQUAL(434, os.offset434(0));
        ASSERT_EQUAL(435, os.offset435(0));
        ASSERT_EQUAL(436, os.offset436(0));
        ASSERT_EQUAL(437, os.offset437(0));
        ASSERT_EQUAL(438, os.offset438(0));
        ASSERT_EQUAL(439, os.offset439(0));

        ASSERT_EQUAL(440, os.offset440(0));
        ASSERT_EQUAL(441, os.offset441(0));
        ASSERT_EQUAL(442, os.offset442(0));
        ASSERT_EQUAL(443, os.offset443(0));
        ASSERT_EQUAL(444, os.offset444(0));
        ASSERT_EQUAL(445, os.offset445(0));
        ASSERT_EQUAL(446, os.offset446(0));
        ASSERT_EQUAL(447, os.offset447(0));
        ASSERT_EQUAL(448, os.offset448(0));
        ASSERT_EQUAL(449, os.offset449(0));

        ASSERT_EQUAL(450, os.offset450(0));
        ASSERT_EQUAL(451, os.offset451(0));
        ASSERT_EQUAL(452, os.offset452(0));
        ASSERT_EQUAL(453, os.offset453(0));
        ASSERT_EQUAL(454, os.offset454(0));
        ASSERT_EQUAL(455, os.offset455(0));
        ASSERT_EQUAL(456, os.offset456(0));
        ASSERT_EQUAL(457, os.offset457(0));
        ASSERT_EQUAL(458, os.offset458(0));
        ASSERT_EQUAL(459, os.offset459(0));

        ASSERT_EQUAL(460, os.offset460(0));
        ASSERT_EQUAL(461, os.offset461(0));
        ASSERT_EQUAL(462, os.offset462(0));
        ASSERT_EQUAL(463, os.offset463(0));
        ASSERT_EQUAL(464, os.offset464(0));
        ASSERT_EQUAL(465, os.offset465(0));
        ASSERT_EQUAL(466, os.offset466(0));
        ASSERT_EQUAL(467, os.offset467(0));
        ASSERT_EQUAL(468, os.offset468(0));
        ASSERT_EQUAL(469, os.offset469(0));

        ASSERT_EQUAL(470, os.offset470(0));
        ASSERT_EQUAL(471, os.offset471(0));
        ASSERT_EQUAL(472, os.offset472(0));
        ASSERT_EQUAL(473, os.offset473(0));
        ASSERT_EQUAL(474, os.offset474(0));
        ASSERT_EQUAL(475, os.offset475(0));
        ASSERT_EQUAL(476, os.offset476(0));
        ASSERT_EQUAL(477, os.offset477(0));
        ASSERT_EQUAL(478, os.offset478(0));
        ASSERT_EQUAL(479, os.offset479(0));

        ASSERT_EQUAL(480, os.offset480(0));
        ASSERT_EQUAL(481, os.offset481(0));
        ASSERT_EQUAL(482, os.offset482(0));
        ASSERT_EQUAL(483, os.offset483(0));
        ASSERT_EQUAL(484, os.offset484(0));
        ASSERT_EQUAL(485, os.offset485(0));
        ASSERT_EQUAL(486, os.offset486(0));
        ASSERT_EQUAL(487, os.offset487(0));
        ASSERT_EQUAL(488, os.offset488(0));
        ASSERT_EQUAL(489, os.offset489(0));

        ASSERT_EQUAL(490, os.offset490(0));
        ASSERT_EQUAL(491, os.offset491(0));
        ASSERT_EQUAL(492, os.offset492(0));
        ASSERT_EQUAL(493, os.offset493(0));
        ASSERT_EQUAL(494, os.offset494(0));
        ASSERT_EQUAL(495, os.offset495(0));
        ASSERT_EQUAL(496, os.offset496(0));
        ASSERT_EQUAL(497, os.offset497(0));
        ASSERT_EQUAL(498, os.offset498(0));
        ASSERT_EQUAL(499, os.offset499(0));



        ASSERT_EQUAL(500, os.offset500(0));
        ASSERT_EQUAL(501, os.offset501(0));
        ASSERT_EQUAL(502, os.offset502(0));
        ASSERT_EQUAL(503, os.offset503(0));
        ASSERT_EQUAL(504, os.offset504(0));
        ASSERT_EQUAL(505, os.offset505(0));
        ASSERT_EQUAL(506, os.offset506(0));
        ASSERT_EQUAL(507, os.offset507(0));
        ASSERT_EQUAL(508, os.offset508(0));
        ASSERT_EQUAL(509, os.offset509(0));

        ASSERT_EQUAL(510, os.offset510(0));
        ASSERT_EQUAL(511, os.offset511(0));
        ASSERT_EQUAL(512, os.offset512(0));
        ASSERT_EQUAL(513, os.offset513(0));
        ASSERT_EQUAL(514, os.offset514(0));
        ASSERT_EQUAL(515, os.offset515(0));
        ASSERT_EQUAL(516, os.offset516(0));
        ASSERT_EQUAL(517, os.offset517(0));
        ASSERT_EQUAL(518, os.offset518(0));
        ASSERT_EQUAL(519, os.offset519(0));

        ASSERT_EQUAL(520, os.offset520(0));
        ASSERT_EQUAL(521, os.offset521(0));
        ASSERT_EQUAL(522, os.offset522(0));
        ASSERT_EQUAL(523, os.offset523(0));
        ASSERT_EQUAL(524, os.offset524(0));
        ASSERT_EQUAL(525, os.offset525(0));
        ASSERT_EQUAL(526, os.offset526(0));
        ASSERT_EQUAL(527, os.offset527(0));
        ASSERT_EQUAL(528, os.offset528(0));
        ASSERT_EQUAL(529, os.offset529(0));

        ASSERT_EQUAL(530, os.offset530(0));
        ASSERT_EQUAL(531, os.offset531(0));
        ASSERT_EQUAL(532, os.offset532(0));
        ASSERT_EQUAL(533, os.offset533(0));
        ASSERT_EQUAL(534, os.offset534(0));
        ASSERT_EQUAL(535, os.offset535(0));
        ASSERT_EQUAL(536, os.offset536(0));
        ASSERT_EQUAL(537, os.offset537(0));
        ASSERT_EQUAL(538, os.offset538(0));
        ASSERT_EQUAL(539, os.offset539(0));

        ASSERT_EQUAL(540, os.offset540(0));
        ASSERT_EQUAL(541, os.offset541(0));
        ASSERT_EQUAL(542, os.offset542(0));
        ASSERT_EQUAL(543, os.offset543(0));
        ASSERT_EQUAL(544, os.offset544(0));
        ASSERT_EQUAL(545, os.offset545(0));
        ASSERT_EQUAL(546, os.offset546(0));
        ASSERT_EQUAL(547, os.offset547(0));
        ASSERT_EQUAL(548, os.offset548(0));
        ASSERT_EQUAL(549, os.offset549(0));

        ASSERT_EQUAL(550, os.offset550(0));
        ASSERT_EQUAL(551, os.offset551(0));
        ASSERT_EQUAL(552, os.offset552(0));
        ASSERT_EQUAL(553, os.offset553(0));
        ASSERT_EQUAL(554, os.offset554(0));
        ASSERT_EQUAL(555, os.offset555(0));
        ASSERT_EQUAL(556, os.offset556(0));
        ASSERT_EQUAL(557, os.offset557(0));
        ASSERT_EQUAL(558, os.offset558(0));
        ASSERT_EQUAL(559, os.offset559(0));

        ASSERT_EQUAL(560, os.offset560(0));
        ASSERT_EQUAL(561, os.offset561(0));
        ASSERT_EQUAL(562, os.offset562(0));
        ASSERT_EQUAL(563, os.offset563(0));
        ASSERT_EQUAL(564, os.offset564(0));
        ASSERT_EQUAL(565, os.offset565(0));
        ASSERT_EQUAL(566, os.offset566(0));
        ASSERT_EQUAL(567, os.offset567(0));
        ASSERT_EQUAL(568, os.offset568(0));
        ASSERT_EQUAL(569, os.offset569(0));

        ASSERT_EQUAL(570, os.offset570(0));
        ASSERT_EQUAL(571, os.offset571(0));
        ASSERT_EQUAL(572, os.offset572(0));
        ASSERT_EQUAL(573, os.offset573(0));
        ASSERT_EQUAL(574, os.offset574(0));
        ASSERT_EQUAL(575, os.offset575(0));
        ASSERT_EQUAL(576, os.offset576(0));
        ASSERT_EQUAL(577, os.offset577(0));
        ASSERT_EQUAL(578, os.offset578(0));
        ASSERT_EQUAL(579, os.offset579(0));

        ASSERT_EQUAL(580, os.offset580(0));
        ASSERT_EQUAL(581, os.offset581(0));
        ASSERT_EQUAL(582, os.offset582(0));
        ASSERT_EQUAL(583, os.offset583(0));
        ASSERT_EQUAL(584, os.offset584(0));
        ASSERT_EQUAL(585, os.offset585(0));
        ASSERT_EQUAL(586, os.offset586(0));
        ASSERT_EQUAL(587, os.offset587(0));
        ASSERT_EQUAL(588, os.offset588(0));
        ASSERT_EQUAL(589, os.offset589(0));

        ASSERT_EQUAL(590, os.offset590(0));
        ASSERT_EQUAL(591, os.offset591(0));
        ASSERT_EQUAL(592, os.offset592(0));
        ASSERT_EQUAL(593, os.offset593(0));
        ASSERT_EQUAL(594, os.offset594(0));
        ASSERT_EQUAL(595, os.offset595(0));
        ASSERT_EQUAL(596, os.offset596(0));
        ASSERT_EQUAL(597, os.offset597(0));
        ASSERT_EQUAL(598, os.offset598(0));
        ASSERT_EQUAL(599, os.offset599(0));


        ASSERT_EQUAL(600, os.offset600(0));
        ASSERT_EQUAL(601, os.offset601(0));
        ASSERT_EQUAL(602, os.offset602(0));
        ASSERT_EQUAL(603, os.offset603(0));
        ASSERT_EQUAL(604, os.offset604(0));
        ASSERT_EQUAL(605, os.offset605(0));
        ASSERT_EQUAL(606, os.offset606(0));
        ASSERT_EQUAL(607, os.offset607(0));
        ASSERT_EQUAL(608, os.offset608(0));
        ASSERT_EQUAL(609, os.offset609(0));

        ASSERT_EQUAL(610, os.offset610(0));
        ASSERT_EQUAL(611, os.offset611(0));
        ASSERT_EQUAL(612, os.offset612(0));
        ASSERT_EQUAL(613, os.offset613(0));
        ASSERT_EQUAL(614, os.offset614(0));
        ASSERT_EQUAL(615, os.offset615(0));
        ASSERT_EQUAL(616, os.offset616(0));
        ASSERT_EQUAL(617, os.offset617(0));
        ASSERT_EQUAL(618, os.offset618(0));
        ASSERT_EQUAL(619, os.offset619(0));

        ASSERT_EQUAL(620, os.offset620(0));
        ASSERT_EQUAL(621, os.offset621(0));
        ASSERT_EQUAL(622, os.offset622(0));
        ASSERT_EQUAL(623, os.offset623(0));
        ASSERT_EQUAL(624, os.offset624(0));
        ASSERT_EQUAL(625, os.offset625(0));
        ASSERT_EQUAL(626, os.offset626(0));
        ASSERT_EQUAL(627, os.offset627(0));
        ASSERT_EQUAL(628, os.offset628(0));
        ASSERT_EQUAL(629, os.offset629(0));

        ASSERT_EQUAL(630, os.offset630(0));
        ASSERT_EQUAL(631, os.offset631(0));
        ASSERT_EQUAL(632, os.offset632(0));
        ASSERT_EQUAL(633, os.offset633(0));
        ASSERT_EQUAL(634, os.offset634(0));
        ASSERT_EQUAL(635, os.offset635(0));
        ASSERT_EQUAL(636, os.offset636(0));
        ASSERT_EQUAL(637, os.offset637(0));
        ASSERT_EQUAL(638, os.offset638(0));
        ASSERT_EQUAL(639, os.offset639(0));

        ASSERT_EQUAL(640, os.offset640(0));
        ASSERT_EQUAL(641, os.offset641(0));
        ASSERT_EQUAL(642, os.offset642(0));
        ASSERT_EQUAL(643, os.offset643(0));
        ASSERT_EQUAL(644, os.offset644(0));
        ASSERT_EQUAL(645, os.offset645(0));
        ASSERT_EQUAL(646, os.offset646(0));
        ASSERT_EQUAL(647, os.offset647(0));
        ASSERT_EQUAL(648, os.offset648(0));
        ASSERT_EQUAL(649, os.offset649(0));

        ASSERT_EQUAL(650, os.offset650(0));
        ASSERT_EQUAL(651, os.offset651(0));
        ASSERT_EQUAL(652, os.offset652(0));
        ASSERT_EQUAL(653, os.offset653(0));
        ASSERT_EQUAL(654, os.offset654(0));
        ASSERT_EQUAL(655, os.offset655(0));
        ASSERT_EQUAL(656, os.offset656(0));
        ASSERT_EQUAL(657, os.offset657(0));
        ASSERT_EQUAL(658, os.offset658(0));
        ASSERT_EQUAL(659, os.offset659(0));

        ASSERT_EQUAL(660, os.offset660(0));
        ASSERT_EQUAL(661, os.offset661(0));
        ASSERT_EQUAL(662, os.offset662(0));
        ASSERT_EQUAL(663, os.offset663(0));
        ASSERT_EQUAL(664, os.offset664(0));
        ASSERT_EQUAL(665, os.offset665(0));
        ASSERT_EQUAL(666, os.offset666(0));
        ASSERT_EQUAL(667, os.offset667(0));
        ASSERT_EQUAL(668, os.offset668(0));
        ASSERT_EQUAL(669, os.offset669(0));

        ASSERT_EQUAL(670, os.offset670(0));
        ASSERT_EQUAL(671, os.offset671(0));
        ASSERT_EQUAL(672, os.offset672(0));
        ASSERT_EQUAL(673, os.offset673(0));
        ASSERT_EQUAL(674, os.offset674(0));
        ASSERT_EQUAL(675, os.offset675(0));
        ASSERT_EQUAL(676, os.offset676(0));
        ASSERT_EQUAL(677, os.offset677(0));
        ASSERT_EQUAL(678, os.offset678(0));
        ASSERT_EQUAL(679, os.offset679(0));

        ASSERT_EQUAL(680, os.offset680(0));
        ASSERT_EQUAL(681, os.offset681(0));
        ASSERT_EQUAL(682, os.offset682(0));
        ASSERT_EQUAL(683, os.offset683(0));
        ASSERT_EQUAL(684, os.offset684(0));
        ASSERT_EQUAL(685, os.offset685(0));
        ASSERT_EQUAL(686, os.offset686(0));
        ASSERT_EQUAL(687, os.offset687(0));
        ASSERT_EQUAL(688, os.offset688(0));
        ASSERT_EQUAL(689, os.offset689(0));

        ASSERT_EQUAL(690, os.offset690(0));
        ASSERT_EQUAL(691, os.offset691(0));
        ASSERT_EQUAL(692, os.offset692(0));
        ASSERT_EQUAL(693, os.offset693(0));
        ASSERT_EQUAL(694, os.offset694(0));
        ASSERT_EQUAL(695, os.offset695(0));
        ASSERT_EQUAL(696, os.offset696(0));
        ASSERT_EQUAL(697, os.offset697(0));
        ASSERT_EQUAL(698, os.offset698(0));
        ASSERT_EQUAL(699, os.offset699(0));


        ASSERT_EQUAL(700, os.offset700(0));
        ASSERT_EQUAL(701, os.offset701(0));
        ASSERT_EQUAL(702, os.offset702(0));
        ASSERT_EQUAL(703, os.offset703(0));
        ASSERT_EQUAL(704, os.offset704(0));
        ASSERT_EQUAL(705, os.offset705(0));
        ASSERT_EQUAL(706, os.offset706(0));
        ASSERT_EQUAL(707, os.offset707(0));
        ASSERT_EQUAL(708, os.offset708(0));
        ASSERT_EQUAL(709, os.offset709(0));

        ASSERT_EQUAL(710, os.offset710(0));
        ASSERT_EQUAL(711, os.offset711(0));
        ASSERT_EQUAL(712, os.offset712(0));
        ASSERT_EQUAL(713, os.offset713(0));
        ASSERT_EQUAL(714, os.offset714(0));
        ASSERT_EQUAL(715, os.offset715(0));
        ASSERT_EQUAL(716, os.offset716(0));
        ASSERT_EQUAL(717, os.offset717(0));
        ASSERT_EQUAL(718, os.offset718(0));
        ASSERT_EQUAL(719, os.offset719(0));

        ASSERT_EQUAL(720, os.offset720(0));
        ASSERT_EQUAL(721, os.offset721(0));
        ASSERT_EQUAL(722, os.offset722(0));
        ASSERT_EQUAL(723, os.offset723(0));
        ASSERT_EQUAL(724, os.offset724(0));
        ASSERT_EQUAL(725, os.offset725(0));
        ASSERT_EQUAL(726, os.offset726(0));
        ASSERT_EQUAL(727, os.offset727(0));
        ASSERT_EQUAL(728, os.offset728(0));
        ASSERT_EQUAL(729, os.offset729(0));

        ASSERT_EQUAL(730, os.offset730(0));
        ASSERT_EQUAL(731, os.offset731(0));
        ASSERT_EQUAL(732, os.offset732(0));
        ASSERT_EQUAL(733, os.offset733(0));
        ASSERT_EQUAL(734, os.offset734(0));
        ASSERT_EQUAL(735, os.offset735(0));
        ASSERT_EQUAL(736, os.offset736(0));
        ASSERT_EQUAL(737, os.offset737(0));
        ASSERT_EQUAL(738, os.offset738(0));
        ASSERT_EQUAL(739, os.offset739(0));

        ASSERT_EQUAL(740, os.offset740(0));
        ASSERT_EQUAL(741, os.offset741(0));
        ASSERT_EQUAL(742, os.offset742(0));
        ASSERT_EQUAL(743, os.offset743(0));
        ASSERT_EQUAL(744, os.offset744(0));
        ASSERT_EQUAL(745, os.offset745(0));
        ASSERT_EQUAL(746, os.offset746(0));
        ASSERT_EQUAL(747, os.offset747(0));
        ASSERT_EQUAL(748, os.offset748(0));
        ASSERT_EQUAL(749, os.offset749(0));

        ASSERT_EQUAL(750, os.offset750(0));
        ASSERT_EQUAL(751, os.offset751(0));
        ASSERT_EQUAL(752, os.offset752(0));
        ASSERT_EQUAL(753, os.offset753(0));
        ASSERT_EQUAL(754, os.offset754(0));
        ASSERT_EQUAL(755, os.offset755(0));
        ASSERT_EQUAL(756, os.offset756(0));
        ASSERT_EQUAL(757, os.offset757(0));
        ASSERT_EQUAL(758, os.offset758(0));
        ASSERT_EQUAL(759, os.offset759(0));

        ASSERT_EQUAL(760, os.offset760(0));
        ASSERT_EQUAL(761, os.offset761(0));
        ASSERT_EQUAL(762, os.offset762(0));
        ASSERT_EQUAL(763, os.offset763(0));
        ASSERT_EQUAL(764, os.offset764(0));
        ASSERT_EQUAL(765, os.offset765(0));
        ASSERT_EQUAL(766, os.offset766(0));
        ASSERT_EQUAL(767, os.offset767(0));
        ASSERT_EQUAL(768, os.offset768(0));
        ASSERT_EQUAL(769, os.offset769(0));

        ASSERT_EQUAL(770, os.offset770(0));
        ASSERT_EQUAL(771, os.offset771(0));
        ASSERT_EQUAL(772, os.offset772(0));
        ASSERT_EQUAL(773, os.offset773(0));
        ASSERT_EQUAL(774, os.offset774(0));
        ASSERT_EQUAL(775, os.offset775(0));
        ASSERT_EQUAL(776, os.offset776(0));
        ASSERT_EQUAL(777, os.offset777(0));
        ASSERT_EQUAL(778, os.offset778(0));
        ASSERT_EQUAL(779, os.offset779(0));

        ASSERT_EQUAL(780, os.offset780(0));
        ASSERT_EQUAL(781, os.offset781(0));
        ASSERT_EQUAL(782, os.offset782(0));
        ASSERT_EQUAL(783, os.offset783(0));
        ASSERT_EQUAL(784, os.offset784(0));
        ASSERT_EQUAL(785, os.offset785(0));
        ASSERT_EQUAL(786, os.offset786(0));
        ASSERT_EQUAL(787, os.offset787(0));
        ASSERT_EQUAL(788, os.offset788(0));
        ASSERT_EQUAL(789, os.offset789(0));

        ASSERT_EQUAL(790, os.offset790(0));
        ASSERT_EQUAL(791, os.offset791(0));
        ASSERT_EQUAL(792, os.offset792(0));
        ASSERT_EQUAL(793, os.offset793(0));
        ASSERT_EQUAL(794, os.offset794(0));
        ASSERT_EQUAL(795, os.offset795(0));
        ASSERT_EQUAL(796, os.offset796(0));
        ASSERT_EQUAL(797, os.offset797(0));
        ASSERT_EQUAL(798, os.offset798(0));
        ASSERT_EQUAL(799, os.offset799(0));


        ASSERT_EQUAL(800, os.offset800(0));
        ASSERT_EQUAL(801, os.offset801(0));
        ASSERT_EQUAL(802, os.offset802(0));
        ASSERT_EQUAL(803, os.offset803(0));
        ASSERT_EQUAL(804, os.offset804(0));
        ASSERT_EQUAL(805, os.offset805(0));
        ASSERT_EQUAL(806, os.offset806(0));
        ASSERT_EQUAL(807, os.offset807(0));
        ASSERT_EQUAL(808, os.offset808(0));
        ASSERT_EQUAL(809, os.offset809(0));

        ASSERT_EQUAL(810, os.offset810(0));
        ASSERT_EQUAL(811, os.offset811(0));
        ASSERT_EQUAL(812, os.offset812(0));
        ASSERT_EQUAL(813, os.offset813(0));
        ASSERT_EQUAL(814, os.offset814(0));
        ASSERT_EQUAL(815, os.offset815(0));
        ASSERT_EQUAL(816, os.offset816(0));
        ASSERT_EQUAL(817, os.offset817(0));
        ASSERT_EQUAL(818, os.offset818(0));
        ASSERT_EQUAL(819, os.offset819(0));

        ASSERT_EQUAL(820, os.offset820(0));
        ASSERT_EQUAL(821, os.offset821(0));
        ASSERT_EQUAL(822, os.offset822(0));
        ASSERT_EQUAL(823, os.offset823(0));
        ASSERT_EQUAL(824, os.offset824(0));
        ASSERT_EQUAL(825, os.offset825(0));
        ASSERT_EQUAL(826, os.offset826(0));
        ASSERT_EQUAL(827, os.offset827(0));
        ASSERT_EQUAL(828, os.offset828(0));
        ASSERT_EQUAL(829, os.offset829(0));

        ASSERT_EQUAL(830, os.offset830(0));
        ASSERT_EQUAL(831, os.offset831(0));
        ASSERT_EQUAL(832, os.offset832(0));
        ASSERT_EQUAL(833, os.offset833(0));
        ASSERT_EQUAL(834, os.offset834(0));
        ASSERT_EQUAL(835, os.offset835(0));
        ASSERT_EQUAL(836, os.offset836(0));
        ASSERT_EQUAL(837, os.offset837(0));
        ASSERT_EQUAL(838, os.offset838(0));
        ASSERT_EQUAL(839, os.offset839(0));

        ASSERT_EQUAL(840, os.offset840(0));
        ASSERT_EQUAL(841, os.offset841(0));
        ASSERT_EQUAL(842, os.offset842(0));
        ASSERT_EQUAL(843, os.offset843(0));
        ASSERT_EQUAL(844, os.offset844(0));
        ASSERT_EQUAL(845, os.offset845(0));
        ASSERT_EQUAL(846, os.offset846(0));
        ASSERT_EQUAL(847, os.offset847(0));
        ASSERT_EQUAL(848, os.offset848(0));
        ASSERT_EQUAL(849, os.offset849(0));

        ASSERT_EQUAL(850, os.offset850(0));
        ASSERT_EQUAL(851, os.offset851(0));
        ASSERT_EQUAL(852, os.offset852(0));
        ASSERT_EQUAL(853, os.offset853(0));
        ASSERT_EQUAL(854, os.offset854(0));
        ASSERT_EQUAL(855, os.offset855(0));
        ASSERT_EQUAL(856, os.offset856(0));
        ASSERT_EQUAL(857, os.offset857(0));
        ASSERT_EQUAL(858, os.offset858(0));
        ASSERT_EQUAL(859, os.offset859(0));

        ASSERT_EQUAL(860, os.offset860(0));
        ASSERT_EQUAL(861, os.offset861(0));
        ASSERT_EQUAL(862, os.offset862(0));
        ASSERT_EQUAL(863, os.offset863(0));
        ASSERT_EQUAL(864, os.offset864(0));
        ASSERT_EQUAL(865, os.offset865(0));
        ASSERT_EQUAL(866, os.offset866(0));
        ASSERT_EQUAL(867, os.offset867(0));
        ASSERT_EQUAL(868, os.offset868(0));
        ASSERT_EQUAL(869, os.offset869(0));

        ASSERT_EQUAL(870, os.offset870(0));
        ASSERT_EQUAL(871, os.offset871(0));
        ASSERT_EQUAL(872, os.offset872(0));
        ASSERT_EQUAL(873, os.offset873(0));
        ASSERT_EQUAL(874, os.offset874(0));
        ASSERT_EQUAL(875, os.offset875(0));
        ASSERT_EQUAL(876, os.offset876(0));
        ASSERT_EQUAL(877, os.offset877(0));
        ASSERT_EQUAL(878, os.offset878(0));
        ASSERT_EQUAL(879, os.offset879(0));

        ASSERT_EQUAL(880, os.offset880(0));
        ASSERT_EQUAL(881, os.offset881(0));
        ASSERT_EQUAL(882, os.offset882(0));
        ASSERT_EQUAL(883, os.offset883(0));
        ASSERT_EQUAL(884, os.offset884(0));
        ASSERT_EQUAL(885, os.offset885(0));
        ASSERT_EQUAL(886, os.offset886(0));
        ASSERT_EQUAL(887, os.offset887(0));
        ASSERT_EQUAL(888, os.offset888(0));
        ASSERT_EQUAL(889, os.offset889(0));

        ASSERT_EQUAL(890, os.offset890(0));
        ASSERT_EQUAL(891, os.offset891(0));
        ASSERT_EQUAL(892, os.offset892(0));
        ASSERT_EQUAL(893, os.offset893(0));
        ASSERT_EQUAL(894, os.offset894(0));
        ASSERT_EQUAL(895, os.offset895(0));
        ASSERT_EQUAL(896, os.offset896(0));
        ASSERT_EQUAL(897, os.offset897(0));
        ASSERT_EQUAL(898, os.offset898(0));
        ASSERT_EQUAL(899, os.offset899(0));



        ASSERT_EQUAL(900, os.offset900(0));
        ASSERT_EQUAL(901, os.offset901(0));
        ASSERT_EQUAL(902, os.offset902(0));
        ASSERT_EQUAL(903, os.offset903(0));
        ASSERT_EQUAL(904, os.offset904(0));
        ASSERT_EQUAL(905, os.offset905(0));
        ASSERT_EQUAL(906, os.offset906(0));
        ASSERT_EQUAL(907, os.offset907(0));
        ASSERT_EQUAL(908, os.offset908(0));
        ASSERT_EQUAL(909, os.offset909(0));

        ASSERT_EQUAL(910, os.offset910(0));
        ASSERT_EQUAL(911, os.offset911(0));
        ASSERT_EQUAL(912, os.offset912(0));
        ASSERT_EQUAL(913, os.offset913(0));
        ASSERT_EQUAL(914, os.offset914(0));
        ASSERT_EQUAL(915, os.offset915(0));
        ASSERT_EQUAL(916, os.offset916(0));
        ASSERT_EQUAL(917, os.offset917(0));
        ASSERT_EQUAL(918, os.offset918(0));
        ASSERT_EQUAL(919, os.offset919(0));

        ASSERT_EQUAL(920, os.offset920(0));
        ASSERT_EQUAL(921, os.offset921(0));
        ASSERT_EQUAL(922, os.offset922(0));
        ASSERT_EQUAL(923, os.offset923(0));
        ASSERT_EQUAL(924, os.offset924(0));
        ASSERT_EQUAL(925, os.offset925(0));
        ASSERT_EQUAL(926, os.offset926(0));
        ASSERT_EQUAL(927, os.offset927(0));
        ASSERT_EQUAL(928, os.offset928(0));
        ASSERT_EQUAL(929, os.offset929(0));

        ASSERT_EQUAL(930, os.offset930(0));
        ASSERT_EQUAL(931, os.offset931(0));
        ASSERT_EQUAL(932, os.offset932(0));
        ASSERT_EQUAL(933, os.offset933(0));
        ASSERT_EQUAL(934, os.offset934(0));
        ASSERT_EQUAL(935, os.offset935(0));
        ASSERT_EQUAL(936, os.offset936(0));
        ASSERT_EQUAL(937, os.offset937(0));
        ASSERT_EQUAL(938, os.offset938(0));
        ASSERT_EQUAL(939, os.offset939(0));

        ASSERT_EQUAL(940, os.offset940(0));
        ASSERT_EQUAL(941, os.offset941(0));
        ASSERT_EQUAL(942, os.offset942(0));
        ASSERT_EQUAL(943, os.offset943(0));
        ASSERT_EQUAL(944, os.offset944(0));
        ASSERT_EQUAL(945, os.offset945(0));
        ASSERT_EQUAL(946, os.offset946(0));
        ASSERT_EQUAL(947, os.offset947(0));
        ASSERT_EQUAL(948, os.offset948(0));
        ASSERT_EQUAL(949, os.offset949(0));

        ASSERT_EQUAL(950, os.offset950(0));
        ASSERT_EQUAL(951, os.offset951(0));
        ASSERT_EQUAL(952, os.offset952(0));
        ASSERT_EQUAL(953, os.offset953(0));
        ASSERT_EQUAL(954, os.offset954(0));
        ASSERT_EQUAL(955, os.offset955(0));
        ASSERT_EQUAL(956, os.offset956(0));
        ASSERT_EQUAL(957, os.offset957(0));
        ASSERT_EQUAL(958, os.offset958(0));
        ASSERT_EQUAL(959, os.offset959(0));

        ASSERT_EQUAL(960, os.offset960(0));
        ASSERT_EQUAL(961, os.offset961(0));
        ASSERT_EQUAL(962, os.offset962(0));
        ASSERT_EQUAL(963, os.offset963(0));
        ASSERT_EQUAL(964, os.offset964(0));
        ASSERT_EQUAL(965, os.offset965(0));
        ASSERT_EQUAL(966, os.offset966(0));
        ASSERT_EQUAL(967, os.offset967(0));
        ASSERT_EQUAL(968, os.offset968(0));
        ASSERT_EQUAL(969, os.offset969(0));

        ASSERT_EQUAL(970, os.offset970(0));
        ASSERT_EQUAL(971, os.offset971(0));
        ASSERT_EQUAL(972, os.offset972(0));
        ASSERT_EQUAL(973, os.offset973(0));
        ASSERT_EQUAL(974, os.offset974(0));
        ASSERT_EQUAL(975, os.offset975(0));
        ASSERT_EQUAL(976, os.offset976(0));
        ASSERT_EQUAL(977, os.offset977(0));
        ASSERT_EQUAL(978, os.offset978(0));
        ASSERT_EQUAL(979, os.offset979(0));

        ASSERT_EQUAL(980, os.offset980(0));
        ASSERT_EQUAL(981, os.offset981(0));
        ASSERT_EQUAL(982, os.offset982(0));
        ASSERT_EQUAL(983, os.offset983(0));
        ASSERT_EQUAL(984, os.offset984(0));
        ASSERT_EQUAL(985, os.offset985(0));
        ASSERT_EQUAL(986, os.offset986(0));
        ASSERT_EQUAL(987, os.offset987(0));
        ASSERT_EQUAL(988, os.offset988(0));
        ASSERT_EQUAL(989, os.offset989(0));

        ASSERT_EQUAL(990, os.offset990(0));
        ASSERT_EQUAL(991, os.offset991(0));
        ASSERT_EQUAL(992, os.offset992(0));
        ASSERT_EQUAL(993, os.offset993(0));
        ASSERT_EQUAL(994, os.offset994(0));
        ASSERT_EQUAL(995, os.offset995(0));
        ASSERT_EQUAL(996, os.offset996(0));
        ASSERT_EQUAL(997, os.offset997(0));
        ASSERT_EQUAL(998, os.offset998(0));
        ASSERT_EQUAL(999, os.offset999(0));

        ASSERT_EQUAL(1000, os.offset1000(0));

    }

} __VirtualOffsetSelector;

