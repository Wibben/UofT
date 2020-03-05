#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
#include "courier_verify.h"

using ece297test::relative_error;
using ece297test::courier_path_is_legal_with_capacity;


SUITE(simple_legality_london_england_public) {
    TEST(simple_legality_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        std::vector<CourierSubpath> result_path;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;

        deliveries = {DeliveryInfo(882, 141018, 156.23058), DeliveryInfo(166023, 415470, 5.90659), DeliveryInfo(177529, 180120, 116.91071), DeliveryInfo(87934, 18095, 196.70969), DeliveryInfo(155842, 362872, 173.25085)};
        depots = {58478, 187180, 30517};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5402.934082031;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(37205, 263120, 178.32225)};
        depots = {9};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(49943, 353427, 25.41876), DeliveryInfo(320395, 254879, 189.14767), DeliveryInfo(323243, 330392, 147.26413), DeliveryInfo(218000, 99605, 187.30554), DeliveryInfo(301185, 110737, 26.07329)};
        depots = {62283, 108139, 55076};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 6835.816894531;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(134454, 15967, 163.30885), DeliveryInfo(139923, 247994, 37.57087), DeliveryInfo(324969, 77004, 150.91125), DeliveryInfo(98809, 363416, 172.39594), DeliveryInfo(76316, 143595, 183.19765), DeliveryInfo(98809, 395386, 196.66364), DeliveryInfo(98809, 321658, 123.80926), DeliveryInfo(134454, 226218, 81.27390)};
        depots = {355126, 52129, 428440};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 9997.939453125;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(158399, 417252, 106.95937), DeliveryInfo(190923, 228140, 122.76128), DeliveryInfo(430464, 138981, 110.45184), DeliveryInfo(430464, 192566, 47.96069), DeliveryInfo(190923, 365443, 10.21251), DeliveryInfo(309467, 427859, 20.93990), DeliveryInfo(430464, 144381, 168.32152), DeliveryInfo(88883, 321635, 189.81805)};
        depots = {24593, 103268, 350877};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2681.585693359;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(203677, 3174, 8.52085), DeliveryInfo(97371, 90214, 109.99455), DeliveryInfo(250051, 50321, 147.01781)};
        depots = {38876, 415172, 236554};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 7268.173339844;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(173430, 41974, 179.33032), DeliveryInfo(283567, 276773, 54.34401), DeliveryInfo(54732, 66189, 39.59011)};
        depots = {111580, 349573, 259485};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 4057.217529297;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(199426, 33349, 25.75946), DeliveryInfo(76362, 193298, 7.45998)};
        depots = {305636, 146512};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 7562.449707031;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(325322, 39181, 112.07799)};
        depots = {115033};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(259206, 40967, 157.95691), DeliveryInfo(266678, 189134, 137.82828)};
        depots = {134964};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 8164.091796875;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(172565, 131200, 127.60234), DeliveryInfo(313131, 109050, 188.37068), DeliveryInfo(93567, 207012, 41.37134), DeliveryInfo(62334, 109050, 168.70045), DeliveryInfo(417826, 158269, 35.80693), DeliveryInfo(255552, 135, 36.45235), DeliveryInfo(320808, 131200, 92.27836), DeliveryInfo(349638, 109050, 139.67783)};
        depots = {170474, 7452, 59356};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 14361.229492188;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(284948, 434935, 50.05916), DeliveryInfo(373700, 119280, 5.78693), DeliveryInfo(121065, 47666, 140.86339), DeliveryInfo(284814, 84154, 16.40904), DeliveryInfo(31219, 80230, 80.75494)};
        depots = {188712, 48546, 329910};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10611.853515625;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(294956, 40871, 13.34065), DeliveryInfo(364347, 170548, 38.17267)};
        depots = {204516, 253346};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2480.979980469;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(434912, 85979, 181.70248), DeliveryInfo(130724, 305741, 160.36668), DeliveryInfo(113141, 85979, 133.86882), DeliveryInfo(209487, 30357, 31.17917), DeliveryInfo(111893, 85979, 67.03266), DeliveryInfo(206272, 358371, 86.91820), DeliveryInfo(131581, 358371, 28.25852), DeliveryInfo(398678, 370756, 60.45930)};
        depots = {112668, 336095, 267144};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 3720.703369141;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(323324, 293937, 41.83202), DeliveryInfo(282130, 245969, 78.48692), DeliveryInfo(282130, 279841, 187.95767), DeliveryInfo(169435, 40770, 126.80535), DeliveryInfo(282130, 359960, 104.87618), DeliveryInfo(258123, 40770, 95.16597), DeliveryInfo(297687, 40770, 73.51675), DeliveryInfo(282130, 359960, 104.87618), DeliveryInfo(258123, 359960, 156.55751)};
        depots = {163445, 267293, 133242};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 1592.769775391;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(160075, 328814, 48.80639), DeliveryInfo(172122, 360940, 132.93713), DeliveryInfo(246549, 329515, 8.27069)};
        depots = {264527, 74443, 305409};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2604.783203125;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(323484, 137447, 87.76869)};
        depots = {269834, 253401};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(373084, 116633, 168.88548), DeliveryInfo(75769, 25536, 71.35764), DeliveryInfo(78677, 380737, 104.14169), DeliveryInfo(277256, 199665, 23.42255), DeliveryInfo(210128, 299999, 29.90889)};
        depots = {321444, 121178, 273562};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2199.411376953;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(396787, 128650, 192.30663), DeliveryInfo(409651, 181426, 32.94259)};
        depots = {393573};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 12869.810546875;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(49689, 380946, 124.45018), DeliveryInfo(294317, 268674, 117.54118)};
        depots = {419409, 433524};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2263.550048828;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(82998, 225326, 79.60168)};
        depots = {423526};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(434180, 252490, 32.78448)};
        depots = {180262, 397682};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(162838, 257963, 193.30107), DeliveryInfo(62348, 182075, 93.65600), DeliveryInfo(273919, 257963, 123.09270), DeliveryInfo(162838, 135726, 193.12202), DeliveryInfo(378483, 135726, 68.79750), DeliveryInfo(402070, 135726, 124.20332), DeliveryInfo(273919, 65219, 7.95288), DeliveryInfo(273919, 69479, 36.20234), DeliveryInfo(273919, 257963, 123.09270)};
        depots = {281868, 98689, 273195};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 1383.799438477;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(343119, 322295, 164.20685), DeliveryInfo(94292, 244979, 29.04740), DeliveryInfo(343119, 391028, 64.30288), DeliveryInfo(252979, 239806, 147.23224), DeliveryInfo(73637, 239806, 181.46185), DeliveryInfo(343119, 92726, 149.13022), DeliveryInfo(73637, 391028, 145.42374), DeliveryInfo(343119, 391028, 64.30288), DeliveryInfo(73210, 239806, 47.43656)};
        depots = {215409, 141111, 295673};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 758.906555176;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(215440, 338157, 14.33238)};
        depots = {262331, 369695};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(175564, 402685, 66.30630), DeliveryInfo(91067, 395480, 60.80866), DeliveryInfo(165349, 295798, 25.59487), DeliveryInfo(10134, 20715, 135.07956), DeliveryInfo(419835, 7962, 89.95691), DeliveryInfo(270537, 395480, 71.59290), DeliveryInfo(178971, 402685, 23.29072), DeliveryInfo(325756, 395480, 94.90450)};
        depots = {411382, 183015, 260890};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 8252.287109375;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(183915, 399980, 185.19974), DeliveryInfo(210284, 410288, 176.71071), DeliveryInfo(337511, 372095, 2.69646), DeliveryInfo(94894, 393385, 161.01949), DeliveryInfo(6737, 137433, 171.41914)};
        depots = {319523, 144005, 397020};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 7150.075683594;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(317752, 422938, 102.34251), DeliveryInfo(129996, 186417, 175.32678)};
        depots = {254753};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 14926.267578125;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(341466, 263162, 190.48668), DeliveryInfo(233016, 223172, 29.51083), DeliveryInfo(398800, 135646, 158.87622), DeliveryInfo(370656, 220599, 156.72679), DeliveryInfo(341466, 129336, 158.52562), DeliveryInfo(233016, 333101, 117.06736), DeliveryInfo(341466, 244991, 182.50621), DeliveryInfo(347184, 429925, 114.41241)};
        depots = {303043, 139599, 362481};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2293.771728516;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(432024, 300375, 121.14141), DeliveryInfo(404802, 210673, 17.10215), DeliveryInfo(367131, 270743, 137.76456), DeliveryInfo(363850, 266331, 33.13564), DeliveryInfo(80462, 316879, 90.37431)};
        depots = {392948, 340291, 259555};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 4075.735107422;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

    } //simple_legality_london_england

} //simple_legality_london_england_public

